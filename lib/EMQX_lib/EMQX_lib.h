#ifndef EMQX_LIBRARIES_H
#define EMQX_LIBRARIES_H

#include <Arduino.h>
#include "WiFi_lib.h"
#include <WiFiClient.h>
#include <PubSubClient.h>

class EMQX {
    public:
        EMQX();

        void MQTTsetup();

        void MQTTRec();
        void MQTTRun();

    private:

};

#endif