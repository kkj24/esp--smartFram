#include "LimitSwitch_Lib.h"

limitSwitch::limitSwitch(){

};

// Setup Limit Switch Pin
void limitSwitch::begin() {
    // FIX: Gunakan INPUT_PULLUP agar pin tidak floating (noise/nilai acak)
    // Dengan PULLUP: HIGH = tidak ditekan, LOW = ditekan
    pinMode(4, INPUT_PULLUP);
    pinMode(14, INPUT_PULLUP);
}

// Read Limit Switch State
void limitSwitch::readBt(bool *BT1, bool *BT2) {
    unsigned long now = millis();
    static unsigned long last = 0;
    const int interval = 50;

    if(now - last >= interval) {
        last = now;

        // FIX: Balik nilai (!digitalRead) karena PULLUP aktif LOW
        // true  = tombol DITEKAN
        // false = tombol TIDAK ditekan
        *BT1 = !digitalRead(14);
        *BT2 = !digitalRead(4);
    }
}