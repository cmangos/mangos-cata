/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_BARADIN_HOLD_H
#define DEF_BARADIN_HOLD_H

enum
{
    MAX_ENCOUNTER               = 3,                        // Argaloth, Occu'tar and Alizabal

    TYPE_ARGALOTH               = 0,
    TYPE_OCCUTAR                = 1,
    TYPE_ALIZABAL               = 2,

    NPC_ARGALOTH                = 47120,                    // all bosses scripted in EventAI.
    NPC_OCCUTAR                 = 52363,
    NPC_ALIZABAL                = 55869,

    GO_ARGALOTH_DOOR            = 207619,
    GO_OCCUTAR_DOOR             = 208953,
    GO_ALIZABAL_DOOR            = 209849,
};

class instance_baradin_hold : public ScriptedInstance
{
    public:
        instance_baradin_hold(Map* pMap);

        void Initialize() override;
        bool IsEncounterInProgress() const override;

        void OnCreatureCreate(Creature* pCreature) override;
        void OnObjectCreate(GameObject* pGo) override;

        void OnCreatureDeath(Creature* pCreature) override;
        void OnCreatureEvade(Creature* pCreature) override;
        void OnCreatureEnterCombat(Creature* pCreature) override;

        void SetData(uint32 uiType, uint32 uiData) override;
        uint32 GetData(uint32 uiType) const override;

        const char* Save() const override { return m_strInstData.c_str(); }
        void Load(const char* chrIn) override;

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;
};

#endif
