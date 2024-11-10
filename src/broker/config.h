#include"sMQTTBroker.h"
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "realme C25s";
const char* password = "code787898";

const char* mqtt_server = "localhost";

const char* mqtt_topic_temp1 = "/dht/temp1";
const char* mqtt_topic_temp2 = "/dht/temp2";
const char* mqtt_topic_temp3 = "/dht/temp3";

const char* mqtt_topic_hum1 = "/dht/lembap1";
const char* mqtt_topic_hum2 = "/dht/lembap2";
const char* mqtt_topic_hum3 = "/dht/lembap3";

const char* mqtt_topic_relay1 = "/relay/1";
const char* mqtt_topic_relay2 = "/relay/2";

WiFiClient espClient;
PubSubClient client(espClient);

sMQTTBroker broker;
#define mqttPort 1883

void setup_broker(){
    broker.init(mqttPort);
}

void setup_client(){
    client.setServer(mqtt_server, mqttPort);
}

IPAddress local_IP(192, 168, 96, 100);        // Alamat IP statis untuk ESP32
IPAddress gateway(192, 168, 1, 1);          // IP router
IPAddress subnet(255, 255, 255, 0);         // Subnet mask

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("Failed to configure Static IP");
    }
    WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println(WiFi.localIP());
    Serial.println("WiFi connected");
}

TaskHandle_t MQTTBrokerTask;
void brokerUpdateTask(void *parameter) {
    while (true) {
        broker.update();
        vTaskDelay(1);
    }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}