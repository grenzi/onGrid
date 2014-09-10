CREATE DATABASE  IF NOT EXISTS `my_db_name` /*!40100 DEFAULT CHARACTER SET latin1 */;
USE `netlab_service_db`;
-- MySQL dump 10.13  Distrib 5.5.16, for Win32 (x86)
--
-- Host: localhost    Database: netlab_service_db
-- ------------------------------------------------------
-- Server version	5.0.95

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Not dumping tablespaces as no INFORMATION_SCHEMA.FILES table on this server
--

--
-- Table structure for table `wkdd_semantic_ranking`
--

DROP TABLE IF EXISTS `wkdd_semantic_ranking`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_semantic_ranking` (
  `id` int(11) NOT NULL auto_increment,
  `semantic_type` varchar(50) NOT NULL,
  `ranking` int(11) NOT NULL,
  PRIMARY KEY  (`id`),
  KEY `semantic_type_id` (`semantic_type`)
) ENGINE=MyISAM AUTO_INCREMENT=134 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_users`
--

DROP TABLE IF EXISTS `wkdd_users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_users` (
  `id` int(30) NOT NULL auto_increment,
  `email` varchar(45) collate utf8_bin NOT NULL,
  `code` varchar(45) character set latin1 NOT NULL,
  `firstname` varchar(45) character set latin1 default NULL,
  `lastname` varchar(45) character set latin1 default NULL,
  `institution` varchar(45) collate utf8_bin default NULL,
  `department` varchar(45) collate utf8_bin default NULL,
  `title` varchar(45) collate utf8_bin default NULL,
  `approved` varchar(45) character set latin1 default 'no',
  `emailverified` varchar(45) collate utf8_bin default 'no',
  `description` varchar(200) collate utf8_bin default NULL,
  `request` int(6) NOT NULL,
  PRIMARY KEY  (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`),
  UNIQUE KEY `email_UNIQUE` (`email`)
) ENGINE=InnoDB AUTO_INCREMENT=54 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_single_jobs`
--

DROP TABLE IF EXISTS `wkdd_single_jobs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_single_jobs` (
  `id` int(11) NOT NULL auto_increment,
  `start_id` varchar(45) character set latin1 default NULL,
  `end_id` varchar(45) character set latin1 default NULL,
  `distance` int(11) default NULL,
  `paths` text character set utf8,
  `status` varchar(45) character set latin1 default NULL,
  `start_time` datetime default NULL,
  `end_time` varchar(45) character set latin1 default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1222 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_semantic_labels`
--

DROP TABLE IF EXISTS `wkdd_semantic_labels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_semantic_labels` (
  `cui` char(8) NOT NULL,
  `semantic_type` varchar(50) NOT NULL,
  KEY `cui_id` (`cui`),
  KEY `semantic_type_id` (`semantic_type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_connected_labelsxref`
--

DROP TABLE IF EXISTS `wkdd_connected_labelsxref`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_connected_labelsxref` (
  `id` int(11) NOT NULL auto_increment,
  `idxref` int(11) default NULL,
  `cui` varchar(45) default NULL,
  `name` varchar(500) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=770435 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_matrix_jobs`
--

DROP TABLE IF EXISTS `wkdd_matrix_jobs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_matrix_jobs` (
  `id` int(11) NOT NULL auto_increment,
  `job_name` varchar(45) collate utf8_bin default NULL,
  `row_filename` varchar(45) character set latin1 default NULL,
  `col_filename` varchar(45) character set latin1 default NULL,
  `special_key` varchar(45) character set latin1 default NULL,
  `retrieval_key` varchar(45) character set latin1 default NULL,
  `status` varchar(45) character set latin1 default NULL,
  `start_time` datetime default NULL,
  `end_time` datetime default NULL,
  `user_email` varchar(45) collate utf8_bin default NULL,
  PRIMARY KEY  (`id`),
  KEY `user_email` (`user_email`),
  CONSTRAINT `matrix_user_email` FOREIGN KEY (`user_email`) REFERENCES `wkdd_users` (`email`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB AUTO_INCREMENT=471 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_ldp_jobs`
--

DROP TABLE IF EXISTS `wkdd_ldp_jobs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_ldp_jobs` (
  `id` int(11) NOT NULL auto_increment,
  `term` varchar(2000) collate utf8_bin default NULL,
  `status` varchar(45) collate utf8_bin default NULL,
  `type` int(11) default NULL,
  `top_cui` varchar(2000) collate utf8_bin default NULL,
  `start_cui_option` varchar(1000) collate utf8_bin default NULL,
  `end_cui_option` varchar(1000) collate utf8_bin default NULL,
  `start_time` datetime default NULL,
  `end_time` datetime default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=240 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_slots`
--

DROP TABLE IF EXISTS `wkdd_slots`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_slots` (
  `id` int(11) NOT NULL auto_increment,
  `thread_id` bigint(20) default NULL,
  `job_id` varchar(45) default NULL,
  `state` tinyint(4) default '0',
  `start_time` datetime default NULL,
  `end_time` datetime default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=21 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `wkdd_isolated_labels`
--

DROP TABLE IF EXISTS `wkdd_isolated_labels`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `wkdd_isolated_labels` (
  `id` int(11) NOT NULL auto_increment,
  `cui` varchar(45) default NULL,
  `name` varchar(500) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=17490 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
