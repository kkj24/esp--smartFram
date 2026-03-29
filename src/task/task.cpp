#include "task/task.h"

EMQX emqx;
WiFi_lib wifi;
TFT_Lib tft;
Mois mois;      // Moisture object
DHT_Lib dht;    // DHT object

// Mois Data from Library Mois
uint8_t moisAv = 0;
uint8_t mois1 = 0;
uint8_t mois2 = 0;
uint8_t mois3 = 0;
uint8_t mois4 = 0;

// Humidity & Temperature from DHT Library
uint8_t hum = 0;
uint8_t temp = 0;

// pH Data
uint8_t pH_soil = 0;

// Read Data Func
void readData() {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 1000;

    if(now - last >= interval) {
        last = now;

        // Read Soil Mois
        mois1 = mois.liveGetRead(0);
        mois2 = mois.liveGetRead(1);
        mois3 = mois.liveGetRead(2);
        mois4 = mois.liveGetRead(3);

        // Get average from All Data
        mois.getAv(&moisAv, mois1, mois2, mois3, mois4);

        // Read DHT Sensor
        dht.readHum(&hum);
        dht.readTemp(&temp);
    }
}

// Send Data
void send() {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 1000;

    if(now - last >= interval) {
        last = now;

        emqx.clearData();

        // Send Mois
        emqx.saveData(mois1, "Mois1:");
        emqx.saveData(mois2, "Mois2:");
        emqx.saveData(mois3, "Mois3:");
        emqx.saveData(mois4, "Mois4:");
        emqx.saveData(moisAv, "Mois_Av:");
        
        // Send DHT
        emqx.saveData(hum, "Hum:");
        emqx.saveData(temp, "Temp:");

        emqx.sendData();
    }
}

// Show Title on TFT Display
void title(uint8_t tab) {
    enum tab {
        TAB1,
        TAB2,
        TAB3
    };
}

// Tab 1 [Show Data Graph]
void tab1(bool show) {
    // While True
    if(show) {
        // Timer Var
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 50;

        // Timer Run
        if(now - last >= interval) {
            last = now; // Timer Update

            tft.soft_pBar(moisAv, 0, );
        }
    }
}

// ===========================
// Automate Set
void autoSet() {
    wifi.setupWiFi();
    emqx.MQTTsetup();
    tft.TFTBegin();
    mois.begin();
    dht.DHTAutoset();
}

// ===========================
// Auto Run Func
void autoRun() {
    readData(); // Read Data
    send();
}