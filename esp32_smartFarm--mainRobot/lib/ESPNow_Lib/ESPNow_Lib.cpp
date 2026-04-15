#include "ESPNow_Lib.h"

// Inisialisasi variabel memori statis untuk buffer data
volatile bool myESPNow::newDataAvailable = false;
DRAM_ATTR char myESPNow::incomingBuffer[250] = {0};
// FIX: Inisialisasi spinlock untuk akses atomik multi-core
portMUX_TYPE myESPNow::mux = portMUX_INITIALIZER_UNLOCKED;

myESPNow::myESPNow() {}

void myESPNow::OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    if(status == ESP_NOW_SEND_SUCCESS) {
        // Serial.println(F("[ESP-NOW] JSON Data Successfully Sent to peer!"));
    } else {
        // Serial.println(F("[ESP-NOW] Delivery Failed (Peer unreachable/Channel mismatch)."));
    }
}

// Callback otomatis ketika data diterima — berjalan di Core 0 (WiFi Task)
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
void myESPNow::OnDataRecv(const esp_now_recv_info_t * esp_now_info, const uint8_t *incomingData, int len) {
#else
void myESPNow::OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
#endif
    if (incomingData == NULL || len <= 0) return; // Mencegah LoadProhibited / Kerusakan Memori
    int processLen = len > 249 ? 249 : len;

    // FIX: Kunci spinlock sebelum mengakses buffer bersama (akses dari Core 0)
    portENTER_CRITICAL_ISR(&mux);
    memcpy((void*)incomingBuffer, incomingData, processLen);
    incomingBuffer[processLen] = '\0';
    newDataAvailable = true;
    portEXIT_CRITICAL_ISR(&mux);
}

// Fungsi eksekusi yang dijalankan di ruang loop() — berjalan di Core 1
void myESPNow::run() {
    // FIX: Cek flag dulu tanpa kunci untuk performa, baru kunci saat copy
    if(!newDataAvailable) return;

    char localBuf[250];

    // FIX: Kunci spinlock saat membaca buffer bersama (akses dari Core 1)
    portENTER_CRITICAL(&mux);
    memcpy(localBuf, (const void*)incomingBuffer, 250);
    newDataAvailable = false; // Turunkan flag di dalam kunci agar atomik
    portEXIT_CRITICAL(&mux);

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, localBuf);
    
    if (error) {
        Serial.print(F("[ESP-NOW] Gagal membedah paket JSON masuk: "));
        Serial.println(error.c_str());
        return;
    }
    
    // Tampilkan dan ekstrak pesan JSON yang diterima
    if(doc["mois_average"].is<int>()) {
        current_moisture = doc["mois_average"].as<int>();
    }

    Serial.println(F("\n======[PESAN ESP-NOW MASUK]======"));
    serializeJsonPretty(doc, Serial); 
    Serial.print(F("\n[DEBUG] Extracted Moisture: ")); Serial.println(current_moisture);
    Serial.println(F("=================================\n"));
}

void myESPNow::begin(const uint8_t* peerAddress) {
    memcpy(peerMac, peerAddress, 6);

    // FIX: Gunakan ESP_OK untuk cek hasil init (lebih eksplisit)
    if (esp_now_init() != ESP_OK) {
        Serial.println(F("[ESP-NOW] Error initializing ESP-NOW"));
        return;
    }

    // Registrasi Callbacks
    esp_now_register_send_cb(OnDataSent);
    esp_now_register_recv_cb(OnDataRecv);

    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, peerMac, 6);
    peerInfo.channel = 0;  // 0 automatically tracks current WiFi channel to prevent disconnecting IoT!
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != 0) {
        Serial.println(F("[ESP-NOW] Failed to add peer"));
        return;
    }

    isRegistered = true;
    Serial.println(F("[ESP-NOW] Initialized and Peer Registered."));
}

bool myESPNow::sendJson(JsonDocument &doc) {
    if(!isRegistered) {
        Serial.println(F("[ESP-NOW] Cannot send, Peer is not registered."));
        return false;
    }

    String jsonString;
    serializeJson(doc, jsonString);

    esp_err_t result = esp_now_send(peerMac, (uint8_t *)jsonString.c_str(), jsonString.length() + 1); // +1 string terminator for easy parse
    
    if (result == 0) {
        return true;
    } else {
        Serial.println(F("[ESP-NOW] Error transmitting data internally."));
        return false;
    }
}
