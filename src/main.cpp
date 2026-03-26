#include <Arduino.h>

#include "task/task.h"

void setup() {
  Serial.begin(115200);
  mois.begin();
  dht.DHTAutoset();

  tft.TFTBegin();
}

void loop() {
  unsigned long waktu = millis();
  static unsigned long waktuSeb = 0;
  int interval = 10;

  static int i = 50;
  static int a = 0;
  static int b = 100;

  if(waktu - waktuSeb >= interval) {
    waktuSeb = waktu;

    i++;
    if(i >= 100)
      i = 0;

    a += 10;
    if(a >= 100)
      a = 0;

    b += 20;
    if(b >= 100)
      b = 0;
    }
    //tft.soft_pBar(i, 10, 10, 50, 10, TFT_WHITE);
    //tft.soft_pBar(i, 10, 30, 50, 10, TFT_WHITE);
    //tft.soft_pBar(i, 10, 50, 50, 10, TFT_WHITE);
    tft.pBar(i, 10, 70, 50, 10, TFT_WHITE);
    tft.pBar(a, 10, 90, 50, 10, TFT_WHITE);
    tft.pBar(b, 10, 110, 50, 10, TFT_WHITE);
}