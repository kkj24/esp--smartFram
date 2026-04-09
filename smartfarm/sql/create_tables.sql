-- ============================================
-- Smart Farm Database Setup
-- ============================================
-- Jalankan SQL ini di phpMyAdmin (http://localhost/phpmyadmin)
-- atau melalui MySQL CLI

-- Buat Database
CREATE DATABASE IF NOT EXISTS smartfarm_db
    CHARACTER SET utf8mb4
    COLLATE utf8mb4_unicode_ci;

USE smartfarm_db;

-- ============================================
-- Tabel utama: sensor_data
-- Menyimpan semua data sensor dari ESP32
-- ============================================
CREATE TABLE IF NOT EXISTS sensor_data (
    id          INT AUTO_INCREMENT PRIMARY KEY,
    mois1       TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Soil Moisture Sensor 1 (%)',
    mois2       TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Soil Moisture Sensor 2 (%)',
    mois3       TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Soil Moisture Sensor 3 (%)',
    mois4       TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Soil Moisture Sensor 4 (%)',
    mois_av     TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Average Soil Moisture (%)',
    humidity    TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Air Humidity (%)',
    temperature TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Temperature (°C)',
    ph          TINYINT UNSIGNED    NOT NULL DEFAULT 0  COMMENT 'Soil pH Level',
    created_at  TIMESTAMP           NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'Data received timestamp',

    -- Index untuk query berdasarkan waktu
    INDEX idx_created_at (created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci
  COMMENT='Sensor data from ESP32 Smart Farm Main Sensor';
