/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
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

/* ScriptData
SDName: instance_baradin_hold
SD%Complete: 20
SDComment: Basic door support
SDCategory: Baradin Hold
EndScriptData */

#include "precompiled.h"
#include "baradin_hold.h"

instance_baradin_hold::instance_baradin_hold(Map* pMap) : ScriptedInstance(pMap)
{
    Initialize();
}

void instance_baradin_hold::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_baradin_hold::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    return false;
}

void instance_baradin_hold::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ARGALOTH:
        case NPC_OCCUTAR:
        case NPC_ALIZABAL:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_baradin_hold::OnObjectCreate(GameObject* pGo)
{
    switch (pGo->GetEntry())
    {
        case GO_ARGALOTH_DOOR:
        case GO_OCCUTAR_DOOR:
        case GO_ALIZABAL_DOOR:
            m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
            break;
    }
}

void instance_baradin_hold::OnCreatureDeath(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ARGALOTH:   SetData(TYPE_ARGALOTH, DONE);   break;
        case NPC_OCCUTAR:    SetData(TYPE_OCCUTAR,  DONE);   break;
        case NPC_ALIZABAL:   SetData(TYPE_ALIZABAL, DONE);   break;
    }
}

void instance_baradin_hold::OnCreatureEvade(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ARGALOTH:   SetData(TYPE_ARGALOTH, FAIL);   break;
        case NPC_OCCUTAR:    SetData(TYPE_OCCUTAR,  FAIL);   break;
        case NPC_ALIZABAL:   SetData(TYPE_ALIZABAL, FAIL);   break;
    }
}

void instance_baradin_hold::OnCreatureEnterCombat(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_ARGALOTH:   SetData(TYPE_ARGALOTH, IN_PROGRESS);   break;
        case NPC_OCCUTAR:    SetData(TYPE_OCCUTAR,  IN_PROGRESS);   break;
        case NPC_ALIZABAL:   SetData(TYPE_ALIZABAL, IN_PROGRESS);   break;
    }
}

void instance_baradin_hold::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_ARGALOTH:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ARGALOTH_DOOR);

            // add / remove encounter frames
            if (Creature* pArgaloth = GetSingleCreatureFromStorage(NPC_ARGALOTH))
                SendEncounterFrame(uiData == IN_PROGRESS ? ENCOUNTER_FRAME_ENGAGE : ENCOUNTER_FRAME_DISENGAGE, pArgaloth->GetObjectGuid());
            break;
        case TYPE_OCCUTAR:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_OCCUTAR_DOOR);

            // add / remove encounter frames
            if (Creature* pOccutar = GetSingleCreatureFromStorage(NPC_OCCUTAR))
                SendEncounterFrame(uiData == IN_PROGRESS ? ENCOUNTER_FRAME_ENGAGE : ENCOUNTER_FRAME_DISENGAGE, pOccutar->GetObjectGuid());
            break;
        case TYPE_ALIZABAL:
            m_auiEncounter[uiType] = uiData;
            DoUseDoorOrButton(GO_ALIZABAL_DOOR);

            // add / remove encounter frames
            if (Creature* pAlizabal = GetSingleCreatureFromStorage(NPC_ALIZABAL))
                SendEncounterFrame(uiData == IN_PROGRESS ? ENCOUNTER_FRAME_ENGAGE : ENCOUNTER_FRAME_DISENGAGE, pAlizabal->GetObjectGuid());
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;
        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " " << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_baradin_hold::GetData(uint32 uiType) const
{
    if (uiType < MAX_ENCOUNTER)
        return m_auiEncounter[uiType];

    return 0;
}

void instance_baradin_hold::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2];

    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_baradin_hold(Map* pMap)
{
    return new instance_baradin_hold(pMap);
}

void AddSC_instance_baradin_hold()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_baradin_hold";
    pNewScript->GetInstanceData = &GetInstanceData_instance_baradin_hold;
    pNewScript->RegisterSelf();
}
