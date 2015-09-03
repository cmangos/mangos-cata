ALTER TABLE db_version CHANGE COLUMN required_c13027_02_mangos_mangos_string required_c13032_01_mangos_command bit;

DELETE FROM command WHERE name='npc addmove';
