ALTER TABLE db_version CHANGE COLUMN required_c13034_03_mangos_creature_movement_template required_c13034_04_mangos_creature bit;

-- Remove waypoints spawned with the old system
DELETE FROM creature WHERE id=1;
