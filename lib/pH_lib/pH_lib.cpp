#include "pH_lib.h"

Adafruit_ADS1115 ads_lib2;

pH_lib::pH_lib() {
    // No Once Function in Here
};

// Setup pH Read
void pH_lib::setupLib() {
    Wire.begin();
    ads_lib2.begin(ADDR2);
    ads_lib2.setDataRate(RATE_ADS1115_128SPS);
    ads_lib2.setGain(GAIN2);

    // Pin Enable
    pinMode(PIN_EN, OUTPUT);
    digitalWrite(PIN_EN, HIGH);
    delay(1000);
    digitalWrite(PIN_EN, LOW); 
}

void pH_lib::pHRead(uint8_t *pH_read) {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 3000;

    static bool state = false;

    static int16_t pH_raw = 0;

    if(now - last >= interval) {
        last = now;

        digitalWrite(PIN_EN, state);

        // Read DMS while EN pin LOW
        if(!state)
            pH_raw = ads_lib2.readADC_SingleEnded(0);

        state = !state; 
    }
    *pH_read = constrain(map(pH_raw, 0, 500, 0, 14), 0, 14);
}

int16_t pH_lib::pH_raw() {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 3000;

    static int16_t get_pH_raw = 0;

    static bool state = false;

    if(now - last >= interval) {
        last = now;
        digitalWrite(PIN_EN, state);

        // Read DMS while EN pin LOW
        if(!state)
            get_pH_raw = ads_lib2.readADC_SingleEnded(0);

        state = !state;
    }
    return get_pH_raw;
}

uint8_t pH_lib::livepH() {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 3000;

    static bool state = false;
    static int16_t pH_raw = 0;

    if(now - last >= interval) {
        last = now;

        digitalWrite(PIN_EN, state);

        // Read DMS while EN pin Low
        if(!state)
            int16_t pH_raw = ads_lib2.readADC_SingleEnded(0);

        state = !state;
    }
    uint8_t pH = constrain(map(pH_raw, 0, 500, 0, 14), 0, 14);
    return pH;
}