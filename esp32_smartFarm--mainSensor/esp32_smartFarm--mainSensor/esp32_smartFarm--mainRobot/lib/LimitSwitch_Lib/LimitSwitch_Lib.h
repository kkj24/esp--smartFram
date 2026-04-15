#ifndef LIMIT_SWITCH_LIBRARIES_H
#define LIMIT_SWITCH_LIBRARIES_H

#include <Arduino.h>

class limitSwitch {
    public:
        limitSwitch();

        void begin();

        void readBt(bool *BT1, bool *BT2);

    private:

};

#endif