#ifndef PH_LIBRARIES_H
#define PH_LIBRARIES_H

#include <Arduino.h>
#include <Adafruit_ADS1X15.h>
#include <Wire.h>

extern Adafruit_ADS1115 ads_lib2;

#ifndef ADDR2
#define ADDR2 0x49
#endif

#ifndef GAIN2
#define GAIN2 GAIN_ONE
#endif

#ifndef PIN_EN
#define PIN_EN 22
#endif

class pH_lib {
    public:
        pH_lib();

        void setupLib();

        int16_t pH_raw();    // Get Raw pH Data
        void pHRead(uint8_t *pH_read);  // read pH

        uint8_t livepH();   // read Live pH

    private:

};

#endif