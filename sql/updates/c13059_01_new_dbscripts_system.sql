
--
-- Table structure for table `dbscripts_on_creature_evade`
--

DROP TABLE IF EXISTS `dbscripts_on_creature_evade`;
CREATE TABLE `dbscripts_on_creature_evade` (
  `id` mediumint(8) unsigned NOT NULL default '0',
  `delay` int(10) unsigned NOT NULL default '0',
  `command` mediumint(8) unsigned NOT NULL default '0',
  `datalong` mediumint(8) unsigned NOT NULL default '0',
  `datalong2` int(10) unsigned NOT NULL default '0',
  `buddy_entry` int(10) unsigned NOT NULL default '0',
  `search_radius` int(10) unsigned NOT NULL default '0',
  `data_flags` tinyint(3) unsigned NOT NULL default '0',
  `dataint` int(11) NOT NULL default '0',
  `dataint2` int(11) NOT NULL default '0',
  `dataint3` int(11) NOT NULL default '0',
  `dataint4` int(11) NOT NULL default '0',
  `x` float NOT NULL default '0',
  `y` float NOT NULL default '0',
  `z` float NOT NULL default '0',
  `o` float NOT NULL default '0',
  `comments` varchar(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



--
-- Table structure for table `dbscripts_on_creature_enter_combat`
--

DROP TABLE IF EXISTS `dbscripts_on_creature_enter_combat`;
CREATE TABLE `dbscripts_on_creature_enter_combat` (
  `id` mediumint(8) unsigned NOT NULL default '0',
  `delay` int(10) unsigned NOT NULL default '0',
  `command` mediumint(8) unsigned NOT NULL default '0',
  `datalong` mediumint(8) unsigned NOT NULL default '0',
  `datalong2` int(10) unsigned NOT NULL default '0',
  `buddy_entry` int(10) unsigned NOT NULL default '0',
  `search_radius` int(10) unsigned NOT NULL default '0',
  `data_flags` tinyint(3) unsigned NOT NULL default '0',
  `dataint` int(11) NOT NULL default '0',
  `dataint2` int(11) NOT NULL default '0',
  `dataint3` int(11) NOT NULL default '0',
  `dataint4` int(11) NOT NULL default '0',
  `x` float NOT NULL default '0',
  `y` float NOT NULL default '0',
  `z` float NOT NULL default '0',
  `o` float NOT NULL default '0',
  `comments` varchar(255) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=utf8;