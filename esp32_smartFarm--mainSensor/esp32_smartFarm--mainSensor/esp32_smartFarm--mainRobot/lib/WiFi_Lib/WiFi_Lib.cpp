#include "WiFi_Lib.h"

String STA_ssid = "";
String STA_pass = "";

myWiFi::myWiFi() {}

bool myWiFi::stateWiFi() {
    static bool state = false;

    if (WiFi.status() != WL_CONNECTED)
        state = true;  // True == Not connected
    else
        state = false; // False == Connected

    return state;
}

void myWiFi::begin() {
    // Mode Dual (AP + STA)
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_ssid, AP_pass);

    // Config
    pref.begin("WiFi_Config", true);
    STA_ssid = pref.getString("ssid_conf", "");
    STA_pass = pref.getString("pass_conf", "");
    pref.end();

    WiFi.setAutoConnect(true);
    WiFi.setAutoReconnect(true);

    // Connect from Sved Data
    WiFi.setHostname("ESP--FAST/mainRobot");
    WiFi.begin(STA_ssid.c_str(), STA_pass.c_str());
    WiFi.reconnect();

    // Start Web Server Handlers
    web.on("/", handleURL);
    web.on("/save", handleSave);
    web.begin();

    Serial.println("\n[WiFi] Started in AP_STA Mode.");
    Serial.print("AP SSID: "); Serial.println(AP_ssid);
    Serial.print("AP PASS: "); Serial.println(AP_pass);
}

void myWiFi::autoRec() {
    bool nowState = stateWiFi();
    static bool lastState = false;

    // Handle Client Web Server
    web.handleClient();

    // Monitoring Serial
    monitDisp();

    // Handle WiFi
    getConf();

    // WiFi Auto Reconnect
    if(nowState) {
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 5000;

        if(now - last >= interval) {
            last = now; // Update Timer

            WiFi.begin(STA_ssid.c_str(), STA_pass.c_str());
            WiFi.reconnect();
        }
    }
}

void myWiFi::monitDisp() {
    bool nowState = stateWiFi();
    static bool lastState = false;
    
    // Disconnect
    if(nowState) {
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        static int Mon_point = 0;   
        static int attempt = 0;     

        if(now - last >= interval) {
            last = now;
            
            if(lastState != nowState) {
                lastState = nowState;
                Serial.print(F("\n=====[WiFi Not Connected -- Disconnected]=====\n"));
                Serial.print(F("Try to Connecting [SSID: "));
                Serial.print(STA_ssid);
                Serial.print(F("]\nConnecting"));
            }

            if(attempt < Timeout) {
                Serial.print(F("."));
                Mon_point++;
                
                if(Mon_point >= 5) {
                    Serial.print(F("\nAttempt: "));
                    attempt++;
                    Serial.print(attempt);
                    Serial.print("\nConnecting");
                    Mon_point = 0;
                }
            }
            
            if(attempt >= Timeout) {
                static bool nowTask = false;
                static bool lastTask = false;

                nowTask = true;
                if(lastTask != nowTask) {
                    lastTask = nowTask;
                    Serial.print(F("\nAttempt: [Time Out]\nESP memuat ulang otomatis...\n"));
                }
                
                ESP.restart(); // timeout
            }
        }
    } else if(!nowState) {
        // Jika Connect
        if(lastState != nowState) {
            lastState = nowState;
            
            Serial.print(F("\nSuccess to Connecting WiFi!"));
            Serial.print(F("\nIP: "));
            Serial.print(WiFi.localIP());
            Serial.print(F("\n============[SSID: "));
            Serial.print(WiFi.SSID());
            Serial.print(F("]============\n"));
        }
    }
}