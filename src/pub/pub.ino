#include "config.h"
#include "DHT.h"

#define DHT1 12
#define DHT3 14

#define RELAY1 26

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
/*
void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Pesan diterima dari topik: ");
    Serial.println(topic);

    String message;
        for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.print("Isi pesan: ");
    Serial.println(message);

    if (String(topic) == mqtt_topic_relay1) {
        if (message == "ON") {
            Serial.println("Relay 1 ON");
            digitalWrite(RELAY1, HIGH);
        } else if (message == "OFF") {
            Serial.println("Relay 1 OFF");
            digitalWrite(RELAY1, LOW);
        }
    }
}
*/

void alarm(){
    if (Temp1 >= 70 or Temp3 >= 70){
        digitalWrite(RELAY1, HIGH);
    }else{
        digitalWrite(RELAY1, LOW);
    }
}

void reconnect() {
  while (!client.connected()) {
    read();
    alarm();
    Serial.print("Connecting to MQTT...");
    if (client.connect("WaterFlowSensor", mqtt_username, mqtt_password)) {
        Serial.println("connected");
    } else {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.print(" - ");
        Serial.println(mqtt_server);
        delay(1000);
    }

  }
}

void setup() {
    Serial.begin(115200);
    dht1.begin();
    dht3.begin();
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    //client.setCallback(callback);

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY1, OUTPUT);

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY1, LOW);
}

void loop() {
    if (!client.connected()) {
        digitalWrite(LED_BUILTIN, LOW);
        reconnect();
    }
    client.loop();
    read();

    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
    client.publish(mqtt_topic_temp1, String(Temp1).c_str());
    client.publish(mqtt_topic_temp3, String(Temp3).c_str());
    client.publish(mqtt_topic_hum1, String(Hum1).c_str());
    client.publish(mqtt_topic_hum3, String(Hum3).c_str());

    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(Temp1);
    delay(200);
}
