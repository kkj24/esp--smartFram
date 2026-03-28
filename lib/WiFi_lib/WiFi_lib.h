#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>

#include "Login_FAST.h"

class WiFi_lib {
    public:
        WiFi_lib();

        void setupWiFi();

        void autoRec(bool allow_restart);
        void dispRec();
        void monitDisp();

        bool stateWiFi();
        int WiFiSignal();

        void Login();

    private:
        // STA Config [NetWork]
        const char *HostName = "ESP--smartFarm--mainSensor";    // ConfigureWiFi Hostname
        
        // AP Config [Login]
        const char *AP_ssid = "ESP--smartFarm--mainSensor[AP-Login]"; // WiFi AP Name
        const char *AP_pass = "ESP--12345678";  // WiFi AP Password
        
        int Timeout = 35;       // Time Out of Attempting
        
};
    
extern String STA_ssid;    // Set STA WiFi Name
extern String STA_pass;    // Set STA WiFi Password

#endif