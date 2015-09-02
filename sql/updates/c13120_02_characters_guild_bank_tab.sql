ALTER TABLE character_db_version CHANGE COLUMN required_c13120_01_characters_guild required_c13120_02_characters_guild_bank_tab bit;

ALTER TABLE `guild_bank_tab` CHANGE `TabText` `TabText` varchar(500) DEFAULT NULL;
