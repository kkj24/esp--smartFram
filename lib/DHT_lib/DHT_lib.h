#ifndef DHT_SENSOR_LIB
#define DHT_SENSOR_LIB

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

//==================
#ifndef DHT_PIN
#define DHT_PIN 13
#endif

#ifndef DHT_TYPE
#define DHT_TYPE DHT22
#endif

extern DHT dht_lib;
//==================

class DHT_Lib {
  public:
    DHT_Lib();

    void DHTAutoset();
    void readTemp(uint8_t *temp);
    void readHum(uint8_t *hum);

  private:

};

#endif