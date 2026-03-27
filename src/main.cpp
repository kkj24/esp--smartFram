#include <Arduino.h>

#include "task/task.h"

void setup() {
  Serial.begin(115200);
  mois.begin();
  dht.DHTAutoset();
  tft.TFTBegin();
}

void loop() {
  

}