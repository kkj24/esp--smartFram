#include <Arduino.h>

#include "MD_Lib.h"
#include "LimitSwitch_Lib.h"
#include "WiFi_Lib.h"
#include "MQTT_Sub.h"
#include "ESPNow_Lib.h"

myESPNow espnow;

SUBS sub;
myWiFi wifi;

MotorDriv mot;
limitSwitch ls;

uint8_t Mac[] = {0xEC, 0xE3, 0x34, 0x1B, 0xAE, 0x08};

const uint8_t PWM_VAL = 180;


void setup() {
  Serial.begin(115200);

  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);

  digitalWrite(32, HIGH);
  digitalWrite(33, HIGH);

  mot.begin();
  wifi.begin();
  sub.begin();
  ls.begin();

  espnow.begin(Mac);
}

void loop() {
  static bool LS1 = false;
  static bool LS2 = false;

  static bool state = false;

  bool wifiDisc = wifi.stateWiFi();
  bool mqttDisc = sub.state();

  espnow.run();

  ls.readBt(&LS2, &LS1);

  int moisture = espnow.current_moisture;

  if(moisture < 45) {
    unsigned long now = millis();
    static unsigned long last = 0;
    int interval = 1000;

    static bool stateDirect = false;

    static bool statePump = false;

    if(now - last >= interval) {
      last = now;

      statePump = !statePump;

      digitalWrite(32, statePump);
      digitalWrite(33, statePump);
    }

    if(LS1 && !LS2) {
      stateDirect = true;
    } else if(LS2 && !LS1) {
      stateDirect = false;
    }

    if(stateDirect) {
      mot.run(0b01010000, PWM_VAL, PWM_VAL);
    } else if(!stateDirect) {
      mot.run(0b10100000, PWM_VAL, PWM_VAL);
    }
  } else if(moisture > 45) {
    mot.run(0b00000000, 0, 0);

    digitalWrite(32, HIGH);
    digitalWrite(33, HIGH);
  }
}