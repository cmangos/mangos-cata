/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "Common.h"
#include "WorldPacket.h"
#include "Log.h"
#include "Corpse.h"
#include "GameObject.h"
#include "Player.h"
#include "ObjectAccessor.h"
#include "ObjectGuid.h"
#include "WorldSession.h"
#include "LootMgr.h"
#include "Object.h"
#include "Group.h"
#include "World.h"
#include "Util.h"
#include "DBCStores.h"
#include "ObjectMgr.h"

void WorldSession::HandleAutostoreLootItemOpcode(WorldPacket& recv_data)
{
    uint8 itemSlot;
    recv_data >> itemSlot;

    DEBUG_LOG("WORLD: CMSG_AUTOSTORE_LOOT_ITEM > requesting item in slot %u", uint32(itemSlot));

    Loot* loot = sLootMgr.GetLoot(_player);

    if (!loot)
    {
        sLog.outError("HandleAutostoreLootItemOpcode> Cannot retrieve loot for player %s", _player->GetObjectGuid().GetString().c_str());
        return;
    }

    ObjectGuid const& lguid = loot->GetLootGuid();

    LootItem* item = loot->GetLootItemInSlot(itemSlot);

    if (!item)
    {
        _player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    // item may not be already looted or blocked by roll system
    if (item->isBlocked || item->lootedBy.find(_player->GetObjectGuid()) != item->lootedBy.end())
    {
        sLog.outError("HandleAutostoreLootItemOpcode> %s already looted itemId(%u)", _player->GetObjectGuid().GetString().c_str(), item->itemId);
        return;
    }

    if (item->lootItemType == LOOTITEM_TYPE_CURRENCY)
    {
        if (CurrencyTypesEntry const * currencyEntry = sCurrencyTypesStore.LookupEntry(item->itemId))
            _player->ModifyCurrencyCount(item->itemId, int32(item->count * currencyEntry->GetPrecision()));

        loot->NotifyItemRemoved(itemSlot, true);
        --loot->maxSlot;
        return;
    }

    // TODO maybe add another loot is allowed for check to be sure no possible cheat
    loot->SendItem(_player, itemSlot);

    if (lguid.IsItem())
    {
        if (Item* item = _player->GetItemByGuid(lguid))
            item->SetLootState(ITEM_LOOT_CHANGED);
    }
}

void WorldSession::HandleLootMoneyOpcode(WorldPacket& /*recv_data*/)
{
    DEBUG_LOG("WORLD: CMSG_LOOT_MONEY");

    Loot* pLoot = sLootMgr.GetLoot(_player);

    if (!pLoot)
    {
        sLog.outError("HandleLootMoneyOpcode> Cannot retrieve loot for player %s", _player->GetObjectGuid().GetString().c_str());
        return;
    }

    pLoot->SendGold(_player);
}

void WorldSession::HandleLootOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("WORLD: CMSG_LOOT");

    ObjectGuid lguid;
    recv_data >> lguid;

    // Check possible cheat
    if (!_player->isAlive())
        return;

    if (Loot* loot = sLootMgr.GetLoot(_player, lguid))
        loot->ShowContentTo(_player);
}

void WorldSession::HandleLootReleaseOpcode(WorldPacket& recv_data)
{
    DEBUG_LOG("WORLD: CMSG_LOOT_RELEASE");

    ObjectGuid lguid;
    recv_data >> lguid;

    if (Loot* loot = sLootMgr.GetLoot(_player, lguid))
        loot->Release(_player);

}

void WorldSession::HandleLootMasterGiveOpcode(WorldPacket& recv_data)
{
    uint8      itemSlot;        // slot sent in LOOT_RESPONSE
    ObjectGuid lootguid;        // the guid of the loot object owner
    ObjectGuid targetGuid;      // the item receiver guid

    recv_data >> lootguid >> itemSlot >> targetGuid;

    Player* target = ObjectAccessor::FindPlayer(targetGuid);
    if (!target)
    {
        sLog.outError("WorldSession::HandleLootMasterGiveOpcode> Cannot retrieve target %s", targetGuid.GetString().c_str());
        return;
    }

    DEBUG_LOG("WorldSession::HandleLootMasterGiveOpcode> Giver = %s, Target = %s.", _player->GetObjectGuid().GetString().c_str(), targetGuid.GetString().c_str());

    Loot* pLoot = sLootMgr.GetLoot(_player, lootguid);

    if (!pLoot)
    {
        sLog.outError("WorldSession::HandleLootMasterGiveOpcode> Cannot retrieve loot for player %s", _player->GetObjectGuid().GetString().c_str());
        return;
    }
    
    if (_player->GetObjectGuid() != pLoot->GetMasterLootGuid())
    {
        sLog.outError("WorldSession::HandleLootMasterGiveOpcode> player %s is not the loot master!", _player->GetObjectGuid().GetString().c_str());
        return;
    }

    InventoryResult msg = pLoot->SendItem(target, itemSlot);

    if (msg != EQUIP_ERR_OK)
    {
        // send duplicate of error massage to master looter
        if (LootItem* lootItem = pLoot->GetLootItemInSlot(itemSlot))
            _player->SendEquipError(msg, NULL, NULL, lootItem->itemId);
        else
            _player->SendEquipError(msg, NULL, NULL);
        return;
    }
}

void WorldSession::HandleLootMethodOpcode(WorldPacket& recv_data)
{
    uint32 lootMethod;
    ObjectGuid lootMaster;
    uint32 lootThreshold;
    recv_data >> lootMethod >> lootMaster >> lootThreshold;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetObjectGuid()))
        return;
    /********************/

    // everything is fine, do it
    group->SetLootMethod((LootMethod)lootMethod);
    group->SetLooterGuid(lootMaster);
    group->SetLootThreshold((ItemQualities)lootThreshold);
    group->SendUpdate();
}

void WorldSession::HandleLootRoll(WorldPacket& recv_data)
{
    ObjectGuid lootedTarget;
    uint32 itemSlot;
    uint8  rollType;
    recv_data >> lootedTarget;                              // guid of the item rolled
    recv_data >> itemSlot;
    recv_data >> rollType;

    sLog.outDebug("WORLD RECIEVE CMSG_LOOT_ROLL, From:%s, rollType:%u", lootedTarget.GetString().c_str(), uint32(rollType));

    Group* group = _player->GetGroup();
    if (!group)
        return;

    if (rollType >= ROLL_NOT_EMITED_YET)
        return;

    sLootMgr.PlayerVote(GetPlayer(), lootedTarget, itemSlot, RollVote(rollType));
}

