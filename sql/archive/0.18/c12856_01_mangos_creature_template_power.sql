ALTER TABLE db_version CHANGE COLUMN required_c12815_01_mangos_creature_template_classlevelstats required_c12856_01_mangos_creature_template_power bit;

ALTER TABLE creature_template CHANGE ManaMultiplier PowerMultiplier float NOT NULL DEFAULT '1';
ALTER TABLE creature_template CHANGE RegenerateHealth RegenerateStats tinyint(3) unsigned NOT NULL DEFAULT '3';
UPDATE creature_template SET RegenerateStats=RegenerateStats|2;
