#ifndef WIFI_LIBRARIES_H
#define WIFI_LIBRARIES_H

#include <Arduino.h>
#include <WiFi.h>

class WiFi_lib {
    public:
        WiFi_lib();

        void setupWiFi();

        void autoRec();
        void dispRec();
        void monitDisp();

        bool stateWiFi();

        void Login();

    private:

};

#endif