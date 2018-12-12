/* Delete default user and its privileges (require MYSQL 5.7, MARIADB 10.1.3) */
DROP USER IF EXISTS `mangos`@`localhost`;

/* Delete default tables */
DROP DATABASE IF EXISTS `catamangos`;
DROP DATABASE IF EXISTS `catacharacters`;
DROP DATABASE IF EXISTS `catarealmd`;
