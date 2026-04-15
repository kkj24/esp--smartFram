#include "ESPNow_lib.h"

// Inisialisasi variabel memori statis untuk penjaga keamanan data
volatile bool ESPNow_Lib::newDataAvailable = false;
char ESPNow_Lib::incomingBuffer[250] = {0};

ESPNow_Lib::ESPNow_Lib() {}

void ESPNow_Lib::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if(status == ESP_NOW_SEND_SUCCESS) {
        // Serial.println(F("[ESP-NOW] Broadcast Data Sensor Sukses!"));
    } else {
        // Serial.println(F("[ESP-NOW] Gagal mengirim data ke robot."));
    }
}

// Callback otomatis ketika JSON terkirim dari Robot!
// CRITICAL FIX: Karena berjalan menumpang di dalam Kernel WiFi Core, ini tidak boleh memuat proses berat.
void ESPNow_Lib::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    if (incomingData == NULL || len <= 0) return; // Mencegah LoadProhibited / Pengecualian Pointer Kosong

    // Pengamanan anti Buffer Overflow (< 250 karakter)
    int processLen = len > 249 ? 249 : len;
    
    // Copy data secara ekstrem cepat ke Memory Utama (RAM) lalu bebas tugaskan Interrupt!
    memcpy((void*)incomingBuffer, incomingData, processLen);
    incomingBuffer[processLen] = '\0';
    
    // Angkat bendera notifikasi
    newDataAvailable = true;
}

// Fungsi eksekusi yang sangat stabil dan aman dari Kernel Panic, dijalankan di ruang loop()
void ESPNow_Lib::run() {
    if(newDataAvailable) {
        // Terapkan Double-Buffering agar buffer tidak corrupted oleh iterupsi mendadak yang baru masuk
        char localBuf[250];
        memcpy(localBuf, (const void*)incomingBuffer, 250);
        newDataAvailable = false;
        
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, localBuf);
        
        if (error) {
            Serial.print(F("[ESP-NOW] Gagal membedah paket JSON dari robot: "));
            Serial.println(error.c_str());
            return;
        }
        
        // Mengekstrak nilai
        Serial.println(F("\n======[PESAN ESP-NOW DARI ROBOT]======"));
        if(doc.containsKey("status_robot")) {
            String msg = doc["status_robot"].as<String>();
            int rpm = doc["kecepatan_motor"].as<int>();
            
            Serial.print(F("Status: ")); Serial.println(msg);
            Serial.print(F("PWM: ")); Serial.println(rpm);
        }
        Serial.println(F("======================================\n"));
    }
}

void ESPNow_Lib::begin(const uint8_t* peerAddress) {
    memcpy(peerMac, peerAddress, 6);

    if (esp_now_init() != 0) { // 0 == ESP_OK
        Serial.println(F("[ESP-NOW] Error initializing ESP-NOW"));
        return;
    }

    // Registrasi Callbacks
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    // Setup Target Peer
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, peerMac, 6);
    peerInfo.channel = 0;  // 0 secara otomatis menempel ke saluran WiFi (Router) saat ini!!
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != 0) {
        Serial.println(F("[ESP-NOW] Failed to add robot peer"));
        return;
    }

    isRegistered = true;
    Serial.println(F("[ESP-NOW] Initialized & Robot Peer Registered."));
}

bool ESPNow_Lib::sendJson(JsonDocument &doc) {
    if(!isRegistered) return false;

    String jsonString;
    serializeJson(doc, jsonString);

    esp_err_t result = esp_now_send(peerMac, (uint8_t *)jsonString.c_str(), jsonString.length() + 1);
    
    if (result == 0) return true; 
    else {
        Serial.println(F("[ESP-NOW] Gagal memancarkan sinyal."));
        return false;
    }
}
