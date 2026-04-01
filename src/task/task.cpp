#include "task/task.h"

EMQX emqx;
WiFi_lib wifi;
TFT_Lib tft;
Mois mois;      // Moisture object
DHT_Lib dht;    // DHT object
pH_lib pH;

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

        pH_soil = pH.pH_raw();
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

        emqx.saveData(pH_soil, "pH:");

        emqx.sendData();
    }
}

// ==============================
// TFT title show
void title() {
    static bool stateLast = false;
    bool nowState = emqx.MQTTState();

    if(stateLast != nowState) {
        stateLast = nowState;

        tft_lib.fillRect(0, 0, 128, 30, tft_lib.color565(64, 64, 64));
        tft_lib.drawFastHLine(0, 30, 128, TFT_WHITE);
        tft.printF(F("FNC Comp"), 15, 2, TFT_YELLOW, 2);
        tft.printF(F("Main Sensor TFT"), 16, 20, TFT_GREENYELLOW);
    }
}

void titleTab(uint8_t tab) {
    enum tabTitle {
        TAB1,
        TAB2,
        TAB3
    };

    switch(tab) {
        // Title tab1
        case TAB1:
            tft.printF(F("Tab1"), 5, 32, TFT_WHITE);
            tft.printF(F("Tab2"), 50, 32, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab3"), 99, 32, tft_lib.color565(64, 64, 64));
        break;

        // Title tab2
        case TAB2:
            tft.printF(F("Tab1"), 5, 32, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab2"), 50, 32, TFT_WHITE);
            tft.printF(F("Tab3"), 99, 32, tft_lib.color565(64, 64, 64));
        break;

        // Title tab3
        case TAB3:
            tft.printF(F("Tab1"), 5, 32, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab2"), 50, 32, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab3"), 99, 32, TFT_WHITE);
        break;
    }
}

// ==============================
// TFT tab1 draw proggress bar
void drawTab1_progressVar() {
    unsigned long now = millis();
    static unsigned long last = 0;
    uint16_t interval = 10;
    title();

    if(now - last >= interval) {
        last = now;

        //tft.printF(F("Mois1: "), )
        //tft.soft_pBar(mois1, 0, 5, 20, 50, 5, TFT_WHITE);
    }
}

// ==============================
// TFT tab1 Show Func
void tab1() {
    drawTab1_progressVar();

}

// ==============================
// Tab 1 [Show Data Graph]
void tabShow(uint8_t show_tab) {
    // Tab State
    enum tab {
        TAB1,
        TAB2,
        TAB3
    };

    switch(show_tab) {
        // Show Tab 1
        case TAB1:

        break;

        // Show Tab 2
        case TAB2:

        break;

        // Show Tab 3
        case TAB3:

        break;
    }
}

// ==============================
// Show TFT Display
void ShowDisp() {

}

// ===========================
// Automate Set
void autoSet() {
    wifi.setupWiFi();
    emqx.MQTTsetup();
    tft.TFTBegin();
    mois.begin();
    dht.DHTAutoset();
    pH.setupLib();
}

// ===========================
// Auto Run Func
void autoRun() {
    readData(); // Read Data
    send();
    tab1(); // Test

    bool bt1 = digitalRead(27);
    bool bt2 = digitalRead(26);
    static bool stateLast1 = false;
    static bool stateLast2 = false;

    static int8_t tab = 0;

    if(!stateLast1 && bt1)
        tab++;

    if(!stateLast2 && bt2)
        tab--;

    stateLast1 = bt1;
    stateLast2 = bt2;

    if(tab >= 3)
        tab = 0;
    if(tab <= 0)
        tab = 3;

    titleTab(tab);
}