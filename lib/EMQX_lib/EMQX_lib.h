#ifndef EMQX_LIBRARIES_H
#define EMQX_LIBRARIES_H

#include <Arduino.h>
#include "WiFi_lib.h"
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

extern WiFiClient espClient;
extern PubSubClient mqtt_lib;
extern JsonDocument doc;

class EMQX {
    public:
        EMQX();

        void MQTTsetup();

        bool MQTTState();

        void run();

        void MQTTRec();
        void SerMon();

        void saveData(uint8_t data, String data_key);
        void clearData();
        void sendData(String topic = "esp32--smartFarm/main-sensor[FNC]");

    private:

};

#endif