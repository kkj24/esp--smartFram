#ifndef ESP_NOW_LIBRARIES_H
#define ESP_NOW_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoJson.h>

class ESPNow_Lib {
    public:
        ESPNow_Lib();

        // Inisialisasi ESP-NOW dan meregistrasi target peer
        void begin(const uint8_t* peerAddress);
        
        // Membungkus dan mengirim data (ArduinoJson) via ESP-NOW 
        bool sendJson(JsonDocument &doc);

        // Menjalankan pengecekan data baru secara aman di main loop()
        void run();

    private:
        uint8_t peerMac[6];
        bool isRegistered = false;
        
        // Safe Data Buffers untuk komunikasi asinkron antara Core 0 (WiFi) dan Core 1 (Main Loop)
        static volatile bool newDataAvailable;
        static char incomingBuffer[250]; 
        
        // Status pengiriman
        static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
        
        // Penerimaan data
        static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
};

#endif
