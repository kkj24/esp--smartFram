#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

extern WebServer web;

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

        int setTimeOut = 50;

    private:
        // STA Config
        const char *HostName = "ESP--smartFarm--mainSensor";
        char *STA_ssid = "test";
        char *STA_pass = "test";

        // AP Config
        const char *AP_ssid = "ESP--smartFarm--mainSensor[AP]";
        const char *AP_pass = "ESP--12345678";

        int Timeout = setTimeOut;       // Time Out of Attempting

};

#endif