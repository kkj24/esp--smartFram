#include <Arduino.h>

#include "task/task.h"

void setup() {
  Serial.begin(115200);

  autoSet();
}

void loop() {
  autoRun();

  yield();
}