#include "Mois_lib.h"

Adafruit_ADS1115 ads_lib;

Mois::Mois() {
};

// Begin Func for Mois Lib
void Mois::begin() {
    Wire.begin();
    ads_lib.begin(ADDR);
    ads_lib.setDataRate(RATE_ADS1115_128SPS);
    ads_lib.setGain(GAIN);
}

// Read Mois Sensor
void Mois::readMois(uint8_t ch, uint8_t *getRead) {
    int16_t rawRead = ads_lib.readADC_SingleEnded(ch);
    uint8_t data = constrain(map(rawRead, -3800, 23450, 100, 0), 0, 100);
    *getRead = data;
}

// Get Average of Mois Sensor
void Mois::getAv(uint8_t *getAv, uint8_t Mois1, uint8_t Mois2, uint8_t Mois3, uint8_t Mois4) {
    if(Mois1 != -1 && Mois2 != -1 && Mois3 != -1 && Mois4 != -1)
        *getAv = (Mois1+Mois2+Mois3+Mois4)/4;
    else if(Mois1 != -1 && Mois2 != -1 && Mois3 != -1)
        *getAv = (Mois1+Mois2+Mois3)/3;
    else if(Mois1 != -1 && Mois2 != -1)
        *getAv = (Mois1+Mois2)/2;
    else if(Mois1 != -1)
        *getAv = Mois1;
}

// Read Mois Sensor Directly
int Mois::liveGetRead(uint8_t ch) {
    int16_t rawRead = ads_lib.readADC_SingleEnded(ch);
    uint8_t readLive = constrain(map(rawRead, -3800, 23450, 100, 0), 0, 100);
    return readLive;
}

// Rea Mois Sensor and returned Raw Data
int Mois::getRawData(uint8_t ch) {
    int16_t rawData = ads_lib.readADC_SingleEnded(ch);
    return rawData;
}