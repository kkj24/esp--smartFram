#include <Arduino.h>

#include "task/task.h"

void setup() {
  Serial.begin(115200);

  autoSet();
}

void loop() {
  wifi.autoRec(false);
  emqx.MQTTRec();

  emqx.run();

  autoRun();
}