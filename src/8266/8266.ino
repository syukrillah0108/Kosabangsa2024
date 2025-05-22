#include "sensor.h"
#include <ESP8266WiFi.h>
#include "TeleBot.h"

const char* ssid = "OSYARA";
const char* password = "omg12345";

void check_alarm() {
    unsigned long currentMillis = millis(); // Mendapatkan waktu saat ini

    if ((Temp1 >= 70 || Temp3 >= 70)) {
        digitalWrite(RELAY1, HIGH);
        if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis; // Mengupdate waktu sebelumnya
            bot.sendMessage(CHATID2,
                "❗PERINGATAN \n Suhu Nori: " + String(int(Temp3)) + " ℃"
                +"\nSuhu Umum: " + String(int(Temp1)) + " ℃"
            );
            Serial.println("Pesan dikirim setiap 10 detik sekali!"); // Mengirim pesan ke Serial Monitor
        }
    } else{
        digitalWrite(RELAY1, LOW);
    }

}

void setup(){
    Serial.begin(115200);
    dht1.begin();
    dht3.begin();
    unsigned long bot_lasttime;

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY1, OUTPUT); 

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY1, LOW);

    WiFi.begin(ssid, password);
    //delay(5000);
    /*
    */
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());


    client_bot.setInsecure();
}
void loop(){

    delay(200);
    digitalWrite(LED_BUILTIN, HIGH);

    read();
    check_alarm();

    if (WiFi.status() == WL_CONNECTED) {
        // Memeriksa update bot secara berkala
        if (millis() > lastTimeBotRan + botInterval) {
            int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            while (numNewMessages) {
                handleNewMessages(numNewMessages);
                numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }
            lastTimeBotRan = millis();
        }
    }else{
        WiFi.begin(ssid, password);
    }
    delay(200);
    digitalWrite(LED_BUILTIN, LOW);
}