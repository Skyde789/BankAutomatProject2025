-- MySQL dump 10.13  Distrib 8.0.41, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: bankatmdb
-- ------------------------------------------------------
-- Server version	8.2.0

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `accounts`
--

DROP TABLE IF EXISTS `accounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `accounts` (
  `account_number` int(11) unsigned zerofill NOT NULL,
  `account_balance` decimal(18,2) DEFAULT '0.00',
  `account_type` enum('debit','credit') NOT NULL,
  `account_credit_limit` int DEFAULT '0',
  `account_customer_id` int NOT NULL,
  `account_state` enum('active','locked') NOT NULL DEFAULT 'active',
  PRIMARY KEY (`account_number`),
  KEY `customer_account` (`account_customer_id`),
  CONSTRAINT `customer_account` FOREIGN KEY (`account_customer_id`) REFERENCES `customers` (`customer_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accounts`
--

LOCK TABLES `accounts` WRITE;
/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
INSERT INTO `accounts` VALUES (00000000001,3000.00,'credit',0,3,'active'),(00000000002,3000.00,'debit',0,4,'active'),(00000000003,3000.00,'debit',0,5,'active');
/*!40000 ALTER TABLE `accounts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cards`
--

DROP TABLE IF EXISTS `cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `cards` (
  `card_number` varchar(30) NOT NULL,
  `card_pin` varchar(255) NOT NULL,
  `card_owner_id` int NOT NULL,
  `card_attempts` tinyint NOT NULL DEFAULT '3',
  `card_state` enum('active','locked') NOT NULL DEFAULT 'active',
  `card_account_number` int(10) unsigned zerofill NOT NULL,
  `card_account_number2` int(10) unsigned zerofill DEFAULT NULL,
  `card_type` enum('single','double') DEFAULT 'single',
  PRIMARY KEY (`card_number`),
  KEY `account_card_idx` (`card_account_number`),
  KEY `account_card2_idx` (`card_account_number2`),
  KEY `customer_card` (`card_owner_id`),
  CONSTRAINT `account_card` FOREIGN KEY (`card_account_number`) REFERENCES `accounts` (`account_number`),
  CONSTRAINT `account_card2` FOREIGN KEY (`card_account_number2`) REFERENCES `accounts` (`account_number`),
  CONSTRAINT `customer_card` FOREIGN KEY (`card_owner_id`) REFERENCES `customers` (`customer_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cards`
--

LOCK TABLES `cards` WRITE;
/*!40000 ALTER TABLE `cards` DISABLE KEYS */;
INSERT INTO `cards` VALUES ('1234','$2b$10$WRjRlIpZ3tuUlfFC6Dooa.FvyI0YRIv7ajwHMLxo5g048Cr7JGAQS',4,3,'active',0000000003,NULL,'single'),('4321','$2b$10$f2ym7n9SpYdJEA1qOOwpQeUYpsMPnR0SHuDgnN7JvrH6SPdWK78JK',3,3,'active',0000000001,0000000002,'double');
/*!40000 ALTER TABLE `cards` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `customers`
--

DROP TABLE IF EXISTS `customers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `customers` (
  `customer_id` int NOT NULL AUTO_INCREMENT,
  `customer_fname` varchar(45) NOT NULL,
  `customer_lname` varchar(45) NOT NULL,
  `customer_address` varchar(45) NOT NULL,
  `customer_phone_number` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`customer_id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customers`
--

LOCK TABLES `customers` WRITE;
/*!40000 ALTER TABLE `customers` DISABLE KEYS */;
INSERT INTO `customers` VALUES (3,'Pentti','Testi3','Testitie 55',NULL),(4,'Seppo','Testi3','Testitie 55',NULL),(5,'Maija','Testi3','Testitie 55',NULL);
/*!40000 ALTER TABLE `customers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transactions`
--

DROP TABLE IF EXISTS `transactions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transactions` (
  `transaction_id` int(11) unsigned zerofill NOT NULL AUTO_INCREMENT,
  `transaction_receiver` int(11) unsigned zerofill DEFAULT NULL,
  `transaction_sender` int(11) unsigned zerofill DEFAULT NULL,
  `transaction_amount` decimal(18,2) NOT NULL,
  `transaction_type` enum('deposit','withdraw','transfer') NOT NULL,
  `transaction_date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`transaction_id`),
  KEY `transaction_receiver_idx` (`transaction_receiver`),
  KEY `transaction_sender_account_idx` (`transaction_sender`),
  CONSTRAINT `transaction_receiver_account` FOREIGN KEY (`transaction_receiver`) REFERENCES `accounts` (`account_number`),
  CONSTRAINT `transaction_sender_account` FOREIGN KEY (`transaction_sender`) REFERENCES `accounts` (`account_number`)
) ENGINE=InnoDB AUTO_INCREMENT=23 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transactions`
--

LOCK TABLES `transactions` WRITE;
/*!40000 ALTER TABLE `transactions` DISABLE KEYS */;
/*!40000 ALTER TABLE `transactions` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-04-03 13:56:21
