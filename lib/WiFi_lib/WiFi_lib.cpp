#include "WiFi_lib.h"

WiFi_lib::WiFi_lib() {};

// Status of WiFi[Connect = false -- Disconnect = true]
bool WiFi_lib::stateWiFi() {
    static bool StateNow = false; // Temporary Storage of WiFiState
    uint8_t Wifistatus = WiFi.status(); // Save WiFi status on 8byte Form

    if(Wifistatus != WL_CONNECTED)
        StateNow = true;  // True == Not connected
    else
        StateNow = false; // False == Connected

    return StateNow;
}

// WiFi Signal Strength Level[Range: 1-5 -- {low-strong}]
int WiFi_lib::WiFiSignal() {
    int signal = WiFi.RSSI();
    signal = constrain(map(signal, -100, -30, 1, 5), 1, 5);

    return signal;
}

void WiFi_lib::autoRec(bool allow_restart) {
    bool nowState = stateWiFi();

    static bool lastState = false;

    if(nowState) {
        // Variable of Timer
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        int Timeout = 50;

        static int point = 0;
        static int attempt = 0;

        if(lastState != nowState) {
            Serial.print(F("Try to Connect: SSID:{\'"));
            Serial.print(WiFi.SSID());
            Serial.print(F("\' \n"));
            Serial.print(F("Connecting"));
        }

        Serial.print(F("."));
        point++;
        
        if(point >= 5) {
            Serial.print("\n");
            attempt++;
            Serial.print(F("Attempt: "));
            Serial.print(attempt);
            Serial.print("\n");
            Serial.print(F("Connecting"));
            point = 0;
        }
        
        if(attempt >= Timeout) {
            Serial.print(F("\nAttempt: [Time Out]\n"));
            Serial.print(F("ESP will be Restart [Yes / No]"));
            if(allow_restart) {
                attempt = 0;
                ESP.restart();
            }
        }
    }
}