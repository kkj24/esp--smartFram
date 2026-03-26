#include "WiFi_lib.h"

WiFi_lib::WiFi_lib() {};

bool WiFi_lib::stateWiFi() {
    static bool StateNow = false; // Temporary Storage of WiFiState
    uint8_t Wifistatus = WiFi.status(); // Save WiFi status on 8byte Form

    if(Wifistatus != WL_CONNECTED)
        StateNow = true;  // True == Not connected
    else
        StateNow = false; // False == Connected

    return StateNow;
}