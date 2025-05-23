#include "DHT.h"

#define DHT1 15//16 //Umum
#define DHT3 4//17 //Nori

#define RELAY1 25//26

#define DHTTYPE DHT21
DHT dht1(DHT1, DHTTYPE);
DHT dht3(DHT3, DHTTYPE);

float Temp1, Temp3;
float Hum1, Hum3;

void read(){
    Temp1 = dht1.readTemperature();
    Temp3 = dht3.readTemperature();

    Hum1 = dht1.readHumidity();
    Hum3 = dht3.readHumidity();
}