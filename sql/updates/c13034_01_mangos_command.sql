ALTER TABLE db_version CHANGE COLUMN required_c13032_01_mangos_command required_c13034_01_mangos_command bit;

DELETE FROM command WHERE name='wp import';
