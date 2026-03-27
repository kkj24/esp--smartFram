#ifndef TASK_FILE_H
#define TASK_FILE_H

#include <Arduino.h>
#include "Mois_lib.h"
#include "DHT_lib.h"
#include "TFT_lib.h"
#include "WiFi_lib.h"

extern WiFi_lib wifi;

extern TFT_Lib tft;

extern Mois mois;
extern DHT_Lib dht;

extern bool task1;
extern bool task2;

extern uint8_t hum;
extern uint8_t temp;

struct time;

void readDHT();
void readMois();

void readButton(bool *button1, bool *button2);

#endif