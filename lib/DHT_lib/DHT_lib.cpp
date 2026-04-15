#include "DHT_lib.h"

DHT dht_lib(DHT_PIN, DHT_TYPE);

DHT_Lib::DHT_Lib() {

};

// Setting up DHT Automate
void DHT_Lib::DHTAutoset() {
    dht_lib.begin();
    delay(100);
}

// Read DHT Air Humidity
void DHT_Lib::readHum(uint8_t *hum) {
    *hum = dht_lib.readHumidity();
}

// Read DHT Air Temperature
void DHT_Lib::readTemp(uint8_t *temp) {
    *temp = dht_lib.readTemperature();
} 

