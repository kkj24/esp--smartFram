#ifndef TASK_FILE_H
#define TASK_FILE_H

#include <Arduino.h>
#include "Mois_lib.h"
#include "DHT_lib.h"
#include "TFT_lib.h"
#include "WiFi_lib.h"
#include "EMQX_lib.h"
#include "pH_lib.h"

extern pH_lib pH;

extern EMQX emqx;

extern WiFi_lib wifi;

extern TFT_Lib tft;

extern Mois mois;
extern DHT_Lib dht;

extern bool task1;
extern bool task2;

void autoSet();
void autoRun();
void tabShow(uint8_t);

#endif