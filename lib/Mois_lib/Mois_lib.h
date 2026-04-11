#ifndef MOISTURE_SENSOR_LIB_H
#define MOISTURE_SENSOR_LIB_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#ifndef ADDR1
#define ADDR1 0x48
#endif

#ifndef GAIN1
#define GAIN1 GAIN_ONE
#endif

extern Adafruit_ADS1115 ads_lib1;

class Mois {
    public:
        Mois();
        
        void begin();
    
        int getRawData(uint8_t ch);     // Get Moisture Read
        int liveGetRead(uint8_t ch);    // Get Moisture Read live
        
        void readMois(uint8_t ch, uint8_t *getRead);    // Read Moisture
        void getAv(uint8_t *getAv, uint8_t Mois1, uint8_t Mois2 = -1, uint8_t Mois3 = -1, uint8_t Mois4 = -1);  // Get Average of Mois
        
    private:
        int16_t min_range = 10;
        int16_t max_range = 26100;
};

#endif