#include "WiFi_lib.h"

WebServer web;

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

// WiFi Auto Reconnect Func
void WiFi_lib::autoRec(bool allow_restart) {
    bool nowState = stateWiFi();

    static bool lastState = false;

    // Monitoring Serial Display Func
    monitDisp();

    if(nowState) {
        // Variable of Timer
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        // Counting Synchronize Var
        static int attempt = 0;        // Var of Attempt Counting

        static int count = 0;   // WiFi Count

        // Timer
        if(now - last >= interval) {
            last = now; // Update Timer

            // Init One Times
            if(lastState != nowState) {
                lastState = nowState; // Update State;

                WiFi.setHostname(HostName); // Configure HostName WiFi
                WiFi.mode(WIFI_AP_STA); // Set WiFi Mode [Dual Mode]
                WiFi.softAP(AP_ssid, AP_pass);  // Set AP mode Config

                WiFi.begin(STA_ssid, STA_pass); // Set STA mode Config    
            }
            
            // Logic of Attempt smaller than Time Out [Attempt < Time Out]
            if(attempt < Timeout) {
            
                count++;    // Counting
                
                // try to Connecting WiFi
                WiFi.begin(STA_ssid, STA_pass); // Set STA mode Config

                // Count Attempt and Reset WiFi Counter
                if(count >= 5) {
                    attempt++;  // Attempt Counting
                    count = 0;  // Reset Serial Monitor Count
                }
            }
            
            // Logic of Attempt greatest than Time Out [Attempt > Time Out]
            if(attempt >= Timeout) {
                // Restart ESP while User Allowing
                if(allow_restart) {
                    attempt = 0;    // Restart Attempt Count
                    ESP.restart();  // Reset ESP
                }
            }
        }
    } 

    // LED Built-In Indicator while Disconnect
    if(nowState) {
        // Timer Var
        unsigned long now = micros();
        static unsigned long last = 0;
        int interval = 1000;

        static uint8_t brightness = 0; // LED brightness

        // Timer Run
        if(now - last >= interval) {
            last = now; // Update Timer

            // Brightness Faded
            brightness++;
            if(brightness >= 255)
                brightness = 0;

            ledcWrite(0, brightness);
        }
    } else if(!nowState) {
        if(lastState != nowState) {
            lastState = nowState;   // Update State

            ledcWrite(0, 0);
        }
    }
}

// WiFi Auto Set Func
void WiFi_lib::setupWiFi() {
    WiFi.setHostname(HostName); // Configure HostName WiFi
    WiFi.mode(WIFI_AP_STA); // Set WiFi Mode [Dual Mode]
    WiFi.softAP(AP_ssid, AP_pass);  // Set AP mode Config
    
    WiFi.begin(STA_ssid, STA_pass); // Set STA mode Config

    // LED Built-In Indicator Setup
    ledcSetup(0, 10000, 8);
    ledcAttachPin(2, 0);

    ledcSetup(2, 0, 8);
    ledcAttachPin(14, 2);
}

// WiFi Auto Display Reconnect to TFT Disp
void WiFi_lib::dispRec() {
    bool nowState = stateWiFi();
    
    // while Disconnect
    if(nowState) {
        // Var of Timer
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        // Timer
        if(now - last >= interval) {
            last = now; // Update Timer

            
        }
        
    // while Connected
    } else if(!nowState) {

    }
}

// WiFi Auto Display to Serial Monitor
void WiFi_lib::monitDisp() {
    bool nowState = stateWiFi();

    static bool lastState = false;
    
    // while Disconnect
    if(nowState) {
        // Var of Timer
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        static int Mon_point = 0;   // Var of Serial Monitor Counting
        
        // Counting Synchronize Var
        static int attempt = 0;        // Var of Attempt Counting

        // Timer
        if(now - last >= interval) {
            last = now; // Update Timer
            
            // Init One Times
            if(lastState != nowState) {
                lastState = nowState;
                
                Serial.print(F("\n=====[WiFi Not Connected -- Disconnected]=====\n"));
                Serial.print(F("Try to Connecting [SSID: "));
                Serial.print(WiFi.SSID());
                Serial.print(F("]\n"));
                Serial.print(F("Connecting"));
            }

            if(attempt < Timeout) {
                // Serial Monitor Progress 
                Serial.print(F("."));
                Mon_point++;    // Serial Monitor Counting
                
                // Serial Monitor Attempting
                if(Mon_point >= 5) {
                    Serial.print(F("\n"));
                    attempt++;  // Attempt Counting
                    Serial.print(F("Attempt: "));
                    Serial.print(attempt);
                    Serial.print("\n");
                    Serial.print(F("Connecting"));
                    Mon_point = 0;  // Reset Serial Monitor Count
                }
            }
            
            // Logic of Attempt greatest than Time Out [Attempt > Time Out]
            if(attempt >= Timeout) {
                static bool nowTask = false;    // Task now
                static bool lastTask = false;   // Task last

                nowTask = true; // Update Task Now

                if(lastTask != nowTask) {
                    lastTask = nowTask; // Update Task

                    // Serial Monitor Reset ESP Time Out
                    Serial.print(F("\nAttempt: [Time Out]\n"));
                    Serial.print(F("ESP will be Restart [Yes / No]\n"));
                }

                // Restart ESP while User Allowing
                if(esp_reset_reason() == ESP_RST_SW) {
                    nowTask = false;    // Restart Task
                    attempt = 0;    // Restart Attempt Count
                }
            }
        }
        // while Connected
    } else if(!nowState) {
        if(lastState != nowState) {
            lastState = nowState;   // Update State
            
            // Serial Monitor Succeed to Connect
            Serial.print(F("\nSuccess to Connecting WiFi!"));
            Serial.print(F("\n============[SSID: "));
            Serial.print(WiFi.SSID());
            Serial.print(F("]============\n"));
        }

        static bool toneState = true;

        if(!nowState && toneState) {
            unsigned long now = millis();
            static unsigned long last = 0;
            int interval = 100;
    
            static int tones = 0;
            int buzzer_tone[3] = {659, 784, 1046};

            if(now - last >= interval) {
                    last = now;
                    
                    ledcWriteTone(2, buzzer_tone[tones]);

                    tones++;
                    
                    if(tones > 3) {
                        toneState = false;
                        ledcWriteTone(2, 0);
                    }
            }
        }
    }
}