<?php
/**
 * MQTT Configuration
 * 
 * Konfigurasi koneksi MQTT untuk Smart Farm
 * Harus sama dengan konfigurasi di ESP32 (EMQX_lib.cpp)
 */

return [
    'host'      => 'broker.emqx.io',       // EMQX public broker (sama dengan ESP32)
    'port'      => 1883,                    // Port MQTT standar
    'username'  => '',                      // Kosong (sama dengan ESP32)
    'password'  => '',                      // Kosong (sama dengan ESP32)
    'client_id' => 'php-smartfarm-' . getmypid(), // Unik per proses
    'topic'     => 'esp32--smartFarm/main-sensor[FNC]', // Topic yang sama dengan ESP32
];
