#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "OSYARA";
const char* password = "omg12345";

const char* mqtt_server = "192.168.100.2";
const char* mqtt_username = "user1";
const char* mqtt_password = "1234567890";

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

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println(WiFi.localIP());
    Serial.println("WiFi connected");
}
