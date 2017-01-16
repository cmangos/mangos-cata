ALTER TABLE db_version CHANGE COLUMN required_c13123_01_mangos_scriptdev2_tables required_12941_01_mangos_event_linkedto BIT(1) NULL DEFAULT NULL;

ALTER TABLE game_event ADD COLUMN linkedTo mediumint(8) unsigned NOT NULL DEFAULT '0' COMMENT 'This event starts only if defined LinkedTo event is started' AFTER holiday;
