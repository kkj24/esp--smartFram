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

    tft_lib.fillRect(0, 0, 128, 30, tft_lib.color565(64, 64, 64));
    tft_lib.drawFastHLine(0, 30, 128, TFT_WHITE);
    tft.printF(F("FNC Comp"), 15, 2, TFT_YELLOW, 2);
    tft.printF(F("Main Sensor TFT"), 16, 20, TFT_GREENYELLOW);
}

// TFT Title Tab
void titleTab(uint8_t tab) {

    const uint8_t TAB1 = 0;
    const uint8_t TAB2 = 1;
    const uint8_t TAB3 = 2;

    uint8_t x = 5;
    uint8_t y = 46;

    switch(tab) {
        // Title tab1
        case TAB1:
            tft.printF(F("Tab1"), x, y, TFT_WHITE);
            tft.printF(F("Tab2"), x * 10, y, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab3"), x * 20 - 1, y, tft_lib.color565(64, 64, 64));
        break;

        // Title tab2
        case TAB2:
            tft.printF(F("Tab1"), x, y, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab2"), x * 10, y, TFT_WHITE);
            tft.printF(F("Tab3"), x * 20 - 1, y, tft_lib.color565(64, 64, 64));
        break;

        // Title tab3
        case TAB3:
            tft.printF(F("Tab1"), x, y, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab2"), x * 10, y, tft_lib.color565(64, 64, 64));
            tft.printF(F("Tab3"), x * 20 - 1, y, TFT_WHITE);
        break;
    }
}

// return color with input range
void colorBar(uint8_t input, uint16_t *getcolor, uint16_t color_min, uint16_t color_av, uint16_t color_max) {
    if(input < 30)
        *getcolor = color_min;
    else if(input < 70)
        *getcolor = color_av;
    else if(input > 70)
        *getcolor = color_max;
}

// ==============================
// TFT tab1 Show Func
void tab1() {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 10;

    const uint8_t x = 2;
    const uint8_t y = 88;
    const uint8_t y_text = 80;

    const uint8_t width = 50;
    const uint8_t height = 5;

    // Get Color
    static uint16_t color_moisAv = TFT_GREEN;
    static uint16_t color_mois1 = TFT_GREEN;
    static uint16_t color_mois2 = TFT_GREEN;
    static uint16_t color_mois3 = TFT_GREEN;
    static uint16_t color_mois4 = TFT_GREEN;
    static uint16_t color_hum = TFT_GREEN;
    static uint16_t color_temp = TFT_GREEN;
    static uint16_t color_ph = TFT_GREEN;

    tft.printF(F("Sensor Log"), 35, 56, tft_lib.color565(98, 38, 115));

    if(now - last >= interval) {
        last = now;

        // Update Color
        colorBar(moisAv, &color_moisAv, TFT_RED, TFT_YELLOW, TFT_GREEN);
        colorBar(mois1, &color_mois1, TFT_RED, TFT_YELLOW, TFT_GREEN);
        colorBar(mois2, &color_mois2, TFT_RED, TFT_YELLOW, TFT_GREEN);
        colorBar(mois3, &color_mois3, TFT_RED, TFT_YELLOW, TFT_GREEN);
        colorBar(mois4, &color_mois4, TFT_RED, TFT_YELLOW, TFT_GREEN);
        colorBar(hum, &color_hum, TFT_RED, TFT_YELLOW, TFT_GREEN);
        colorBar(temp, &color_temp, TFT_GREEN, TFT_YELLOW, TFT_RED);

        // draw proggress bar
        tft.printF(F("Mois Av"), x, y_text, TFT_WHITE);
        tft.soft_pBar(moisAv, 0, x, y, width, height, color_moisAv);

        tft.println(F("Mois1"), x + 75, y_text);
        tft.soft_pBar(mois1, 1, x + 75, y, width, height, color_mois1);

        tft.println(F("Mois2"), x, y_text + 8 + 6);
        tft.soft_pBar(mois2, 2, x, y + 8 + 6, width, height, color_mois2);

        tft.println(F("Mois3"), x + 75, y_text + 8 + 6);
        tft.soft_pBar(mois3, 3, x + 75, y + 8 + 6, width, height, color_mois3);

        tft.println(F("Mois4"), x, y_text + 8 + 6 + 8 + 6);
        tft.soft_pBar(mois4, 4, x, y + 8 + 6 + 8 + 6, width, height, color_mois4);

        tft.println(F("Hum"), x + 75, y_text + 8 + 6 + 8 + 6);
        tft.soft_pBar(hum, 5, x + 75, y + 8 + 6 + 8 + 6, width, height, color_hum);

        tft.println(F("Temp"), x, y_text + 8 + 6 + 8 + 6 + 8 + 6);
        tft.soft_pBar(temp, 6, x, y + 8 + 6 + 8 + 6 + 8 + 6, width, height, color_temp);

        tft.println(F("pH"), x + 75, y_text + 8 + 6 + 8 + 6 + 8 + 6);
        tft.soft_pBar(pH_soil, 7, x + 75, y + 8 + 6 + 8 + 6 + 8 + 6, width, height, color_ph);
    }
}

// =============================
// TFT Tab Show Function
void tab2() {
    tft.printF(F("Raw data Log"), 30, 56, TFT_DARKCYAN);

    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 500;

    static int16_t last_Mois1 = 0;

    if(now - last >= interval) {
        last = now;
        tft_lib.fillRect(35, 69, 50, 120, TFT_BLACK);

        tft.println(F("Mois1: "), 2, 69);
        tft.print(String(mois.getRawData(0)));

        tft.println(F("Mois2: "), 2, 77);
        tft.print(String(mois.getRawData(1)));

        tft.println(F("Mois3: "), 2, 85);
        tft.print(String(mois.getRawData(2)));
        
        tft.println(F("Mois4: "), 2, 93);
        tft.print(String(mois.getRawData(3)));

        static uint8_t hum = 0;
        dht.readHum(&hum);

        tft.println(F("Humid: "), 2, 109);
        tft.print(String(hum));

        static uint8_t temp = 0;
        dht.readTemp(&temp);

        tft.println(F("Temp:  "), 2, 118);
        tft.print(String(temp));

        tft.println(F("pH:    "), 2, 134);
        tft.print(String(pH_soil));
    }
}

// =============================
// TFT Tab Show Function
void tab3() {
    tft.printF(F("Network Log"), 30, 56, TFT_DARKGREEN);

    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 1000;

    if(now - last >= interval) {
        last = now;

        tft.printF(F("IP : "), 2, 70, tft_lib.color565(110, 125, 125));
        tft.print(WiFi.localIP().toString());
    }
}

// ==============================
// Show TFT Display Icon
void ShowDispIcon() {
    // draw line
    tft_lib.drawFastHLine(0, 44, 128, TFT_WHITE);
    tft_lib.drawFastHLine(0, 54, 128, tft_lib.color565(125, 125, 125));

    // Show Icon of WiFi and MQTT

    int8_t wifi_sig = WiFi.RSSI();
    static int8_t last_sig = 0;

    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 500;

    if(now - last >= interval){
        last = now;

        tft.MQTTstate_Icon(5, 32, true);
        tft.printF(F("Online"), 18, 34, TFT_CYAN);

        static uint16_t color = 0;

        if(wifi.WiFiSignal() == 1)
            color = TFT_RED;
        else if(wifi.WiFiSignal() == 2)
            color = TFT_ORANGE;
        else if(wifi.WiFiSignal() == 3)
            color = TFT_YELLOW;
        else if(wifi.WiFiSignal() == 4)
            color = TFT_GREEN;
        else if(wifi.WiFiSignal() == 5)
            color = tft_lib.color565(107, 76, 245);
        
        if(last_sig != wifi_sig) {
            last_sig = wifi_sig;
            tft_lib.fillRect(82, 34, 64, 8, TFT_BLACK);
        }

        tft.WiFiLevel_Icon(70, 32, wifi.WiFiSignal());
        tft.printF(String(wifi_sig), 83, 34, color);
        tft.print(F(" dBm"));
    }
}

// ==============================
// Show TFT raw Icon (Static)
void staticIcon() {
    int8_t wifi_sig = WiFi.RSSI();
    static int8_t last_sig = 0;

    tft.MQTTstate_Icon(5, 32, true);
    tft.printF(F("Online"), 18, 34, TFT_CYAN);

    static uint16_t color = 0;

    if(wifi.WiFiSignal() == 1)
        color = TFT_RED;
    else if(wifi.WiFiSignal() == 2)
        color = TFT_ORANGE;
    else if(wifi.WiFiSignal() == 3)
        color = TFT_YELLOW;
    else if(wifi.WiFiSignal() == 4)
        color = TFT_GREEN;
    else if(wifi.WiFiSignal() == 5)
        color = tft_lib.color565(107, 76, 245);
    
    if(last_sig != wifi_sig) {
        last_sig = wifi_sig;
        tft_lib.fillRect(82, 34, 64, 8, TFT_BLACK);
    }

    tft.WiFiLevel_Icon(70, 32, wifi.WiFiSignal());
    tft.printF(String(wifi_sig), 83, 34, color);
    tft.print(F(" dBm"));
}

// Run TFT Display
void runTAB() {
    bool bt1 = digitalRead(27);
    bool bt2 = digitalRead(26);
    static bool stateLast1 = false;
    static bool stateLast2 = false;
    
    // tab func
    static int8_t tab = 0;

    const uint8_t TAB1 = 0;
    const uint8_t TAB2 = 1;
    const uint8_t TAB3 = 2;
    
    if(!stateLast1 && bt1) {
        tab++;
        tft_lib.fillScreen(TFT_BLACK);
        title();
        staticIcon();
    }
    
    else if(!stateLast2 && bt2) {
        tab--;
        tft_lib.fillScreen(TFT_BLACK);
        title();
        staticIcon();
    }
        
    stateLast1 = bt1;
    stateLast2 = bt2;

    if(tab > 2)
        tab = 0;
    if(tab < 0)
        tab = 2;

    // Show title
    titleTab(tab);
    // Show Icon
    ShowDispIcon();

    switch(tab) {
        case TAB1:
            tab1();
        break;

        case TAB2:
            tab2();
        break;

        case TAB3:
            tab3();
            break;
    }

    static bool titlestate = false;
    static bool titlestateLast = false;

    titlestate = true;

    if(titlestateLast != titlestate) {
        titlestateLast = titlestate;
        title();
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
    pH.setupLib();

    pinMode(26, INPUT);
    pinMode(27, INPUT);
}

// ===========================
// Auto Run Func
void autoRun() {
    bool WIFIStatus = wifi.stateWiFi();
    bool MQTTState = emqx.MQTTState();

    if(!MQTTState && !WIFIStatus) {
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        static uint8_t count = 0;
        static bool stateTask = false;

        if(now - last >= interval) {
            last = now;

            count++;
            if(count >= 5) {
                count = 5;
                stateTask = true;
            }
        }

        if(stateTask) {
            readData(); // Read Data
            send();
            runTAB();
        }
    }
    
}