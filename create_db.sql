-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema micro_project
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `micro_project` ;

-- -----------------------------------------------------
-- Schema micro_project
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `micro_project` DEFAULT CHARACTER SET utf8 ;
USE `micro_project` ;

-- -----------------------------------------------------
-- Table `micro_project`.`history`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `micro_project`.`history` ;

CREATE TABLE IF NOT EXISTS `micro_project`.`history` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `temperature` DOUBLE NOT NULL,
  `humidity` DOUBLE NOT NULL,
  `hour` VARCHAR(2) NOT NULL,
  `minute` VARCHAR(2) NOT NULL,
  `sec` VARCHAR(2) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
