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

    private:

};

#endif