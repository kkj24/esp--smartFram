#ifndef MQTT_SUBSCRIBER_LIBRARIES_H
#define MQTT_SUBSCRIBER_LIBRARIES_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "WiFi_Lib.h"

class SUBS {
    public:
        SUBS();

        // Setup and Initializations
        void begin();

        // Function that must be run in the loop() routinely to handle incoming payloads
        void run();

        // Auto reconnect to broker and connectivity (non-blocking)
        void autoRec();

        bool state();

        // Methods to grab JSON values directly by assigning them to existing variables automatically
        void getValTo(String key, int16_t &val);
        void getValTo(String key, float &val);
        void getValTo(String key, String &val);

        // Immediate returning function for directly calling variables
        int16_t getInt(String key);
        float getFloat(String key);
        String getString(String key);

    private:
        const char *MQTT_SERVER = "broker.emqx.io";
        const int MQTT_PORT = 1883;
        const char *MQTT_TOPIC = "esp32--smartFarm/main-sensor[FNC]";

        bool MQTTState();
        void SerMon();
};

#endif