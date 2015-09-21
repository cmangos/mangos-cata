ALTER TABLE db_version CHANGE COLUMN required_c12729_01_mangos_playercreateinfo_spell required_c12739_01_mangos_command bit;

UPDATE `command` SET help = 'Syntax: .account create $account $password [$expansion]\r\n\r\nCreate account and set password to it. Optionally, you may also set another expansion for this account than the defined default value.' WHERE name = 'account create';
