#ifndef ESP_NOW_LIBRARIES_H
#define ESP_NOW_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_idf_version.h>
#include <ArduinoJson.h>

class myESPNow {
    public:
        myESPNow();

        // Initiation requires the Mac Address of the receiving peer
        void begin(const uint8_t* peerAddress);
        
        // Wrap and send ArduinoJson format via ESP-NOW 
        bool sendJson(JsonDocument &doc);

        // Menjalankan pengecekan data baru secara aman di main loop()
        void run();

        // Accessible variable for other modules
        int current_moisture = 0;

    private:
        uint8_t peerMac[6];
        bool isRegistered = false;
        
        // Safe Data Buffers dengan mutex untuk komunikasi asinkron antara Core WiFi (Core 0) dan Main Loop (Core 1)
        // FIX: DRAM_ATTR memastikan buffer ada di DRAM yang aman diakses dari WiFi callback/ISR
        static volatile bool newDataAvailable;
        static DRAM_ATTR char incomingBuffer[250];
        // FIX: Spinlock untuk akses atomik, mencegah race condition antara OnDataRecv dan run()
        static portMUX_TYPE mux;

        static void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
        static void OnDataRecv(const esp_now_recv_info_t * esp_now_info, const uint8_t *incomingData, int len);
#else
        static void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
#endif
};

#endif
