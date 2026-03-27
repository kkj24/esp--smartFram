#include "task/task.h"

TFT_Lib tft;
Mois mois;      // Moisture object
DHT_Lib dht;    // DHT object

// Timer structur
struct timer {
    uint16_t interval;
    unsigned long last;
};

// =======================================
//              Read DHT
timer t1 = {1000, 0};   // Timer1

uint8_t hum = 0;    // Hum
uint8_t temp = 0;   // Temp

// DHT Read Func
void readDHT() {
    unsigned long nowTime = millis();
    
    if(nowTime - t1.last >= t1.interval) {
        t1.last = nowTime;
        
        dht.readHum(&temp); // Read Temperature
        dht.readHum(&hum);  // Read Humidity
    }
}

// ========================================
//              Read Moisture
timer t2 = {1000, 0};   // Timer 2

uint8_t mois1 = 0;      // Mois1
uint8_t mois2 = 0;      // Mois2
uint8_t mois3 = 0;      // Mois3
uint8_t mois4 = 0;      // Mois4
uint8_t mois_Av = 0;    // Mois Average

int16_t mois_Raw1 = 0;  // Raw Mois1
int16_t mois_Raw2 = 0;  // Raw Mois2
int16_t mois_Raw3 = 0;  // Raw Mois3
int16_t mois_Raw4 = 0;  // Raw Mois4
int16_t mois_Av_Raw = 0;// Raw Mois Average

// Read Mois Func
void readMois() {
    unsigned long nowTime = millis();

    uint8_t ch1 = 0;    // Channel1
    uint8_t ch2 = 1;    // Channel2
    uint8_t ch3 = 2;    // Channel3
    uint8_t ch4 = 3;    // Channel4

    if(nowTime - t2.last >= t2.interval) {
        t2.last = nowTime;

        mois.readMois(ch1, &mois1);     // Read Mois1 Channel0
        mois.readMois(ch2, &mois2);     // Read Mois2 Channel1
        mois.readMois(ch3, &mois3);     // Read Mois3 Channel2
        mois.readMois(ch4, &mois4);     // Read Mois4 Channel3
        mois.getAv(&mois_Av, mois1, mois2, mois3, mois4);     // Average data of Mois Read

        mois_Raw1 = mois.getRawData(ch1);   // Read raw data Mois1 Channel0
        mois_Raw2 = mois.getRawData(ch2);   // Read raw data Mois2 Channel1
        mois_Raw3 = mois.getRawData(ch3);   // Read raw data Mois3 Channel2
        mois_Raw4 = mois.getRawData(ch4);   // Read raw data Mois4 Channel3
        mois_Av_Raw = (mois_Raw1 + mois_Raw2 + mois_Raw3 + mois_Raw4) / 4;   // Average raw data of Mois Read
    }
}

void task() {
    
}