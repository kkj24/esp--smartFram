#include <Arduino.h>

#include "task/task.h"

void setup() {
  Serial.begin(115200);
  mois.begin();
  dht.DHTAutoset();
  tft.TFTBegin();

  wifi.setupWiFi();
  emqx.MQTTsetup();
}

void loop() {
  wifi.autoRec(false);
  emqx.MQTTRec();

  emqx.run();

  unsigned long now = millis();
  static unsigned long last = 0;
  int interval = 5000;

  if(now - last >= interval) {
    last = now;
    emqx.sendData(90, "test:"); 
  }
}