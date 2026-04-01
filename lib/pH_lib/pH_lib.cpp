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
    digitalWrite(PIN_EN, LOW);
    digitalWrite(PIN_EN, HIGH);
}

void pH_lib::pHRead(uint8_t *pH_read) {
    int16_t pH_raw = ads_lib2.readADC_SingleEnded(0);
    //*pH_read = constrain(map(pH_raw))
}

int16_t pH_lib::pH_raw() {
    int16_t get_pH_raw = ads_lib2.readADC_SingleEnded(0);
    return get_pH_raw;
}

uint8_t pH_lib::livepH() {
    int16_t pH_raw = ads_lib2.readADC_SingleEnded(0);
    //uint8_t pH = constrain(map(pH_raw, 0))
    return pH_raw;
}