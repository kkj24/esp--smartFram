#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include "Login_FAST.h"

extern String STA_ssid;
extern String STA_pass;

class myWiFi {
    public:
        myWiFi();

        void begin();
        void autoRec();
        
        bool stateWiFi();
        void monitDisp();

    private:
        // AP Config [Login]
        const char *AP_ssid = "ESP--smartFarm--mainRobot"; 
        const char *AP_pass = "ESP--FAST/mainRobot";  
        
        int Timeout = 35;       // Time Out of Attempting
};

#endif