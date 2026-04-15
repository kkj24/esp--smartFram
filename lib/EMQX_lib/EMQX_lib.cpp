#include "EMQX_lib.h"

WiFiClient espClient;
PubSubClient mqtt_lib(espClient);
JsonDocument doc;

const int MQTT_Port = 1883; // Port MQTT
const char *MQTT_server = "broker.emqx.io"; // Server Domain
const char *MQTT_Username = ""; // MQTT username [optional]
const char *MQTT_Pass = ""; // MQTT password [optional]

EMQX::EMQX() {
    // No Once Function
};

// Run EMQX Func
void EMQX::run() {
    mqtt_lib.loop();
}

// Setup MQTT Automate Func
void EMQX::MQTTsetup() {
    mqtt_lib.setServer(MQTT_server, MQTT_Port);
}

// MQTT Auto Reconnect
void EMQX::MQTTRec() {
    bool stateMqtt = MQTTState();

    static bool lastState = true;

    // Display Serial Monitor
    SerMon();

    // FIX: Alokasikan ID MQTT hanya sekali pada memori statis agar tidak menyebabkan Heap Corruption / Fragmentation!
    static String ID = "";
    if (ID == "") {
        ID = "ESP--smartFarm" + String(WiFi.macAddress());
    }

    // Reconnect MQTT
    if(stateMqtt) {
        unsigned long now = millis();
        static unsigned long last = 0;
        uint16_t interval = 5000;

        if(now - last >= interval) {
            last = now;
            mqtt_lib.connect(ID.c_str());
        }
    }

    // Tones of Reconnect - Connect
    if(stateMqtt || !stateMqtt) {
        static bool state_tone = true;

        // While Disconnect
        if(state_tone && stateMqtt) {
            // Timer Var
            unsigned long now = millis();
            static unsigned long last = 0;
            uint8_t interval = 100;

            const uint16_t tone_buz[] = {440, 349, 292}; // Tones List
            static uint8_t prog_tone = 0;   // Progressive Tone

            // Timer of Tone
            if(now - last >= interval) {
                last = now;

                // Udate Tone
                ledcWriteTone(2, tone_buz[prog_tone]);
                prog_tone++;

                // Reset Tone
                if(prog_tone > 3) {
                    prog_tone = 0;
                    ledcWriteTone(2, 0);
                    state_tone = false;
                }
            }
        // While Connected
        } else if(!state_tone && !stateMqtt) {
            // Timer Var
            unsigned long now = millis();
            static unsigned long last = 0;
            uint8_t interval = 70;

            //tone ref: D5 - D5, D6, A5, #G5, G5, F5, D5, F5, G5
            uint16_t _D5 = 587;
            uint16_t _F5 = 698;
            uint16_t _G5 = 784;
            uint16_t _GS5 = 830;
            uint16_t _A5 = 880;
            uint16_t _D6 = 1174;
            const uint16_t buz_tones[] = {_D5, 0, _D5, 0, _D6, 0, 0, _A5, 0, 0, 0, _GS5, 0, 0, _G5, 0, 0, _F5, 0, 0, _D5, 0, _F5, 0, _G5};    // Tones List
            static uint8_t prog_tones = 0;  // Progressive Tone

            // Timer Run
            if (now - last >= interval) {
                last = now; // Update Timer

                // Update Tone
                ledcWriteTone(2, buz_tones[prog_tones]);
                prog_tones++;

                // Reset Tone
                if(prog_tones > 25 ) {
                    prog_tones = 0;
                    ledcWriteTone(2, 0);
                    state_tone = true;
                }
            }
        }
    }
    
    // LED Built-In Indicator
    if(stateMqtt) {
        unsigned long now = micros();
        static unsigned long last = 0;
        uint16_t interval = 1000;

        static uint8_t brightness = 255;

        if(now - last >= interval) {
            last = now;

            ledcWrite(0, brightness);

            brightness--;
            if(brightness <= 0)
                brightness = 255;
        }
    } else {
        bool now_led = stateMqtt;
        static bool last_led = true;
        if(last_led != now_led) {
            lastState = stateMqtt;
            ledcWrite(0, 0);
        }
    }
}

// Save Data with JSON Form
char dataSend[200];
void EMQX::saveData(int16_t data, String data_key) {
    if(!mqtt_lib.connected())
        return;

    doc[data_key] = data;

    serializeJson(doc, dataSend, sizeof(dataSend));
}

// Clear JSON Data to Prevent Over Flow
void EMQX::clearData() {
    doc.clear();
}

// Publish All Data into MQTT Server
void EMQX::sendData(String topic) {
    mqtt_lib.publish(topic.c_str(), dataSend);
}

// MQTT Status [False == Connected -- True == Disconnect]
bool EMQX::MQTTState() {
    static bool state = false;

    // Update Status
    if(WiFi.status() == WL_CONNECTED) {
        // Timer Var
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        static uint8_t count = 0;   // Count Var

        // Run Timer of Update Status
        if(now - last >= interval) {
            last = now;     // Update Timer

            count++;    // Counting

            // Reset Count
            if(count > 1) {
                count = 0;
                if(mqtt_lib.state() == MQTT_DISCONNECTED)
                    state = true;
                else if(mqtt_lib.state() == MQTT_CONNECTED)
                    state = false;
            }
        }
    }
    // Return Status
    return state;
}

// Serail Monitor of MQTT
void EMQX::SerMon() {
    bool nowState = MQTTState();
    static bool lastState = false;

    static String status_rec = "";

    // Monitor Once Time
    
    // Serial Monitor Progress of Reconnect
    if(nowState) {
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;
        
        static uint8_t point = 0;
        static uint16_t attempt = 0;
        
        if(now - last >= interval) {
            last = now;

            // Status Update
            if(nowState) {
                if(lastState != nowState) {
                    lastState = nowState;
        
                    Serial.print(F("\nMQTT Not Connected\n"));
                    Serial.print(F("Status: "));
                    Serial.print(mqtt_lib.state());
                    Serial.print(F(" -- "));

                    switch(mqtt_lib.state()) {
                        // Failed to Connect
                        case MQTT_CONNECT_FAILED:
                            Serial.print(F("MQTT Failed To Connecting"));
                        break;
        
                        // Connection Lost
                        case MQTT_CONNECTION_LOST:
                            Serial.print(F("MQTT was Lost Connection"));
                        break;
        
                        // Connection TimeOut
                        case MQTT_CONNECTION_TIMEOUT:
                            Serial.print(F("MQTT Connection was Time Out"));
                            break;
                            
                        // MQTT Disconnected
                        case MQTT_DISCONNECTED:
                            Serial.print(F("MQTT Disconnect"));
                        break;
                    }

                    Serial.print(F("\n"));
                    Serial.print(F("Connecting"));
                }
            }

            // Update Counting
            Serial.print(F("."));
            point++;

            // Reset Counting
            if(point >= 5) {
                point = 0;
                attempt++;

                Serial.print(F("\nAttempt: "));
                Serial.print(attempt);
                Serial.print(F("\nConnecting"));
            }
        }
    }

    // Monitor else Once Time
    if(!nowState) {
        if(lastState != nowState) {
            lastState = nowState;

            Serial.print(F("\n"));
            Serial.print(F("\n====["));
            Serial.print(F("MQTT Connected!"));
            Serial.print(F("]====\n"));
        }
    }
}