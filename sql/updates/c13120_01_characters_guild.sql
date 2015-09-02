ALTER TABLE character_db_version CHANGE COLUMN required_c12952_01_characters_pvpstats required_c13120_01_characters_guild bit;

ALTER TABLE `guild` CHANGE `info` `info` varchar(500) NOT NULL DEFAULT '';
ALTER TABLE `guild` CHANGE `motd` `motd` varchar(128) NOT NULL DEFAULT '';
