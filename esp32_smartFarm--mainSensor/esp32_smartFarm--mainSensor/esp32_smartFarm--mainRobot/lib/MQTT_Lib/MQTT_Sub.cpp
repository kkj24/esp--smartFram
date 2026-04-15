#include "MQTT_Sub.h"

WiFiClient esp_robot;
PubSubClient dom(esp_robot);
JsonDocument doc_sub;

SUBS::SUBS() {}

// CallBack designed to be strictly non-blocking
void callBack(char* topic, byte* payLoad, unsigned int length) {
    // Deserialize to the latest JsonDocument
    DeserializationError error = deserializeJson(doc_sub, payLoad, length);
    
    if(error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }
}

void SUBS::begin() {
    dom.setServer(MQTT_SERVER, MQTT_PORT);
    dom.setCallback(callBack);
}

void SUBS::run() {
    // Must be called inside void loop() to allow data to flow smoothly
    dom.loop();
}

// ---------------- RETRIEVAL METHODS (Direct Fetching) ----------------

int16_t SUBS::getInt(String key) {
    // Return JSON data according to the key. If not found, returns 0 (safe default integer)
    return doc_sub[key.c_str()] | 0;
}

float SUBS::getFloat(String key) {
    // Return JSON data according to the key. If not found, returns 0.0 (safe default float)
    return doc_sub[key.c_str()] | 0.0f;
}

String SUBS::getString(String key) {
    // Return JSON data as string according to the key. If not found, returns ""
    return doc_sub[key.c_str()] | "";
}

// ---------------- OVERLOADED VARIABLES BINDING ----------------

void SUBS::getValTo(String key, int16_t &val) {
    if (doc_sub[key.c_str()].is<int16_t>()) {
        val = doc_sub[key.c_str()].as<int16_t>();
    }
}

void SUBS::getValTo(String key, float &val) {
    if (doc_sub[key.c_str()].is<float>()) {
        val = doc_sub[key.c_str()].as<float>();
    }
}

void SUBS::getValTo(String key, String &val) {
    if (doc_sub[key.c_str()].is<const char*>()) {
        val = doc_sub[key.c_str()].as<String>();
    }
}

// ---------------- MQTT STATE AND RECONNECT ----------------

bool SUBS::MQTTState() {
    static bool state = true; // True means Disconnected

    if(WiFi.status() == WL_CONNECTED) {
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;

        static uint8_t count = 0;

        if(now - last >= interval) {
            last = now;
            count++;

            if(count > 1) {
                count = 0;
                if(dom.state() == MQTT_DISCONNECTED)
                    state = true;
                else if(dom.state() == MQTT_CONNECTED)
                    state = false;
            }
        }
    }
    return state;
}

void SUBS::autoRec() {
    bool stateMqtt = MQTTState();
    
    // Serial debug status connection
    SerMon();

    // FIX: Alokasikan ID MQTT hanya sekali pada memori statis agar tidak menyebabkan Heap Corruption / Fragmentation!
    static String ID = "";
    if (ID == "") {
        ID = "ESP--smartFarm_robot_" + String(WiFi.macAddress());
    }

    // Reconnect non-blocking
    if(stateMqtt && WiFi.status() == WL_CONNECTED) {
        unsigned long now = millis();
        static unsigned long last = 0;
        uint16_t interval = 5000; // Delay for reconnect attempt

        if(now - last >= interval) {
            last = now;
            if(dom.connect(ID.c_str())) {
                // Once connected, immediately subscribe to the topic
                dom.subscribe(MQTT_TOPIC);
            }
        }
    }
}

void SUBS::SerMon() {
    bool nowState = MQTTState();
    static bool lastState = false;

    // Disconnected Mode
    if(nowState) {
        unsigned long now = millis();
        static unsigned long last = 0;
        int interval = 1000;
        
        static uint8_t point = 0;
        static uint16_t attempt = 0;
        
        if(now - last >= interval) {
            last = now;

            if(lastState != nowState) {
                lastState = nowState;
                Serial.print(F("\n[MQTT Robot] Disconnected/Not Connected\n"));
                Serial.print(F("Connecting to: "));
                Serial.print(MQTT_SERVER);
                Serial.print(F("\n"));
                Serial.print(F("Connecting"));
            }

            Serial.print(F("."));
            point++;

            if(point >= 5) {
                point = 0;
                attempt++;
                Serial.print(F("\nAttempt: "));
                Serial.print(attempt);
                Serial.print(F("\nConnecting"));
            }
        }
    } else {
        // Connected Mode
        if(lastState != nowState) {
            lastState = nowState;

            Serial.print(F("\n"));
            Serial.print(F("\n======["));
            Serial.print(F("MQTT Connected & Subscribed to Sensor!"));
            Serial.print(F("]======\n"));
        }
    }
}

bool SUBS::state() {
    static bool state = false;
    // FIX: Tambahkan return statement yang hilang (sebelumnya menyebabkan Undefined Behavior / Crash!)
    if (!dom.connected())
         state = true;  // true = Disconnected (konsisten dengan stateWiFi)
    else
        state = false; // false = Connected

    return state;
}