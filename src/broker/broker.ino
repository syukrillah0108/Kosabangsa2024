#include "config.h"
#include "DHT.h"

#include <HardwareSerial.h>

#define DHT1 27
#define DHT3 14

#define RELAY1 26
#define btn 25

#define DHTTYPE DHT21
DHT dht1(DHT1, DHTTYPE);
DHT dht3(DHT3, DHTTYPE);

float Temp1, Temp3;
float Hum1, Hum3;

HardwareSerial sim800(1);

// Daftar nomor yang diizinkan (ganti dengan nomor Anda)
const String allowedNumbers[] = {"+6285774244205", "+6281234567890", "+6289876543210"};
const int allowedNumbersCount = sizeof(allowedNumbers) / sizeof(allowedNumbers[0]);

String senderNumber = "";  // Variabel untuk menyimpan nomor pengirim
String message = "";       // Variabel untuk menyimpan isi pesan

void read(){
    Temp1 = dht1.readTemperature();
    Temp3 = dht3.readTemperature();

    Hum1 = dht1.readHumidity();
    Hum3 = dht3.readHumidity();
}

bool pos_btn = true;

void alarm(){
    if ((Temp1 >= 70 or Temp3 >= 70) && pos_btn == true){
        digitalWrite(RELAY1, HIGH);
    }else{
        digitalWrite(RELAY1, LOW);
        pos_btn = true;
    }
}

void sendCommand(String command) {
  sim800.println(command);
  delay(1000); // Tunggu respon
  while (sim800.available()) {
    Serial.write(sim800.read());
  }
}

// Fungsi untuk mengekstrak nomor pengirim dari notifikasi +CMT
String extractSenderNumber(String data) {
  int startIdx = data.indexOf("\"") + 1; // Posisi pertama setelah tanda kutip
  int endIdx = data.indexOf("\"", startIdx); // Posisi tanda kutip berikutnya
  return data.substring(startIdx, endIdx);
}

// Fungsi untuk membalas SMS
void replySMS(String phoneNumber, String replyMessage) {
  sim800.println("AT+CMGS=\"" + phoneNumber + "\""); // Kirim perintah mengirim SMS
  delay(1000);
  sim800.print(replyMessage); // Kirim isi pesan
  delay(100);
  sim800.write(26); // ASCII Ctrl+Z untuk mengirim pesan
  Serial.println("Reply sent to: " + phoneNumber);
}

// Fungsi untuk menghentikan panggilan telepon
void hangUpCall() {
  sim800.println("ATH"); // Perintah untuk menghentikan panggilan
  Serial.println("Call ended");
}

// Fungsi untuk cek status panggilan
void checkCallStatus() {
  sim800.println("AT+CLCC"); // Cek status panggilan aktif
  delay(1000);
  while (sim800.available()) {
    String callStatus = sim800.readString();
    Serial.println("Call Status: " + callStatus);
    if (callStatus.indexOf("ACTIVE") != -1) {
      Serial.println("Call in progress...");
    }
  }
  hangUpCall();
}

// Fungsi untuk melakukan panggilan telepon
void callNumber(String phoneNumber) {
  Serial.println("Calling: " + phoneNumber);
  sim800.println("ATD" + phoneNumber + ";"); // Perintah untuk memanggil nomor
  delay(2000); // Beri waktu untuk memulai panggilan
  checkCallStatus(); // Cek status panggilan
}

// Fungsi untuk mengecek apakah nomor pengirim diizinkan
bool isAllowedNumber(String phoneNumber) {
  for (int i = 0; i < allowedNumbersCount; i++) {
    if (phoneNumber == allowedNumbers[i]) {
      return true;
    }
  }
  return false;
}

// Fungsi untuk mengirim pesan ke semua nomor yang ada di array
void sendBroadcastSMS(String broadcastMessage) {
  for (int i = 0; i < allowedNumbersCount; i++) {
    String phoneNumber = allowedNumbers[i];
    replySMS(phoneNumber, broadcastMessage);
    delay(1000); // Tambahkan delay agar tidak terlalu cepat mengirim pesan berikutnya
  }
  Serial.println("Broadcast SMS sent to all allowed numbers.");
}

void setup(){
    Serial.begin(115200);
    sim800.begin(9600, SERIAL_8N1, 16, 17); // UART1, RX=16, TX=17

      // Inisialisasi modul
    sendCommand("AT");
    delay(1000);
    sendCommand("AT+CMGF=1"); // Mode teks
    delay(1000);
    sendCommand("AT+CNMI=2,2,0,0,0"); // Menampilkan SMS langsung
    dht1.begin();
    dht3.begin();

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY1, OUTPUT);
    pinMode(btn, INPUT);

    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(RELAY1, LOW);

    setup_wifi();
    setup_broker();
    xTaskCreatePinnedToCore(
        brokerUpdateTask,   // Task function
        "MQTTBrokerTask",   // Task name
        4096,               // Stack size (4 KB)
        NULL,               // Parameter (not used here)
        1,                  // Priority
        &MQTTBrokerTask,    // Task handle
        0                   // Core 0
    );
    setup_client();
}
void loop(){
    /*
    if (!client.connected()) {
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
    //Serial.println(Temp1);
    delay(200);
    */
  if (sim800.available()) {
    String response = sim800.readStringUntil('\n');

    // Cek apakah SMS diterima dengan notifikasi +CMT
    if (response.startsWith("+CMT:")) {
      // Ambil nomor pengirim
      senderNumber = extractSenderNumber(response);
      Serial.println("Sender Number: " + senderNumber);

      // Ambil isi pesan
      message = sim800.readStringUntil('\n');
      Serial.println("Message: " + message);

      // Cek apakah nomor pengirim diizinkan
      if (isAllowedNumber(senderNumber)) {
        Serial.println("Authorized sender. Responding...");

        // Balas SMS
        //replySMS(senderNumber, "Terima kasih, pesan Anda telah diterima!");
        delay(1000);

        // Jika pesan berisi kata "CALL", lakukan panggilan
        if (message.indexOf("CALL") >= 0) {
          sendBroadcastSMS("Kirim");
        }
      } else {
        Serial.println("Unauthorized sender. Ignoring message.");
      }
    }
  }
}