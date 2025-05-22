#include"sMQTTBroker.h"
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "OSYARA";
const char* password = "omg12345";

const char* mqtt_server = "localhost";

const char* mqtt_topic_temp1 = "/dht/temp1";
const char* mqtt_topic_temp2 = "/dht/temp2";
const char* mqtt_topic_temp3 = "/dht/temp3";

const char* mqtt_topic_hum1 = "/dht/lembap1";
const char* mqtt_topic_hum2 = "/dht/lembap2";
const char* mqtt_topic_hum3 = "/dht/lembap3";

const char* mqtt_topic_relay1 = "/relay/1";
const char* mqtt_topic_relay2 = "/relay/2";

const char* topic1 = "esp32/suhu";
const char* topic2 = "esp32/kelembaban";
const char* topic3 = "esp32/status";

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

// Deklarasi Handle Task
TaskHandle_t WiFiCheckTaskHandle = NULL;

void connectToWiFi() {
    Serial.print("Menghubungkan ke Wi-Fi ");
    Serial.println(ssid);
    /*
    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("Failed to configure Static IP");
    }
    */
    WiFi.begin(ssid, password);

    // Menunggu hingga koneksi berhasil
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        vTaskDelay(500 / portTICK_PERIOD_MS); // Delay 500 ms
    }
    Serial.println("\nTerhubung ke Wi-Fi!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
}


TaskHandle_t MQTTBrokerTask;
void brokerUpdateTask(void *parameter) {
    while (true) {
        broker.update();
        vTaskDelay(100/portTICK_PERIOD_MS);
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