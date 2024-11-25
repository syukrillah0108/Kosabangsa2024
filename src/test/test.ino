#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// ====== Konfigurasi WiFi ======
const char* ssid = "realme C25s";
const char* password = "code787898";

// ====== Konfigurasi Telegram Bot ======
#define BOT_TOKEN "7679786493:AAGuYqA8evAvANNHvMIydReW0aPGWP4ICSQ" // Ganti dengan Bot Token kamu
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

// Variabel untuk memeriksa update bot
unsigned long lastTimeBotRan;
const long botInterval = 1000; // Interval 1 detik

// LED pin
const int ledPin = 2; // Pin LED (biasanya built-in LED ada di pin 2)

// Fungsi untuk menyambungkan ke WiFi
void connectWiFi() {
  Serial.print("Menyambungkan ke WiFi ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nTerhubung ke WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// Fungsi untuk menangani perintah Telegram
void handleNewMessages(int numNewMessages) {
  Serial.println("Pesan baru diterima:");

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;

    if (from_name == "") from_name = "User";

    Serial.print("Pesan dari ");
    Serial.print(from_name);
    Serial.print(": ");
    Serial.println(text);

    // Respon terhadap perintah
    if (text == "/start") {
      String welcome = "Halo, " + from_name + "!\n";
      welcome += "Ini adalah ESP32 Telegram Bot.\n\n";
      welcome += "Ketik /led_on untuk menyalakan LED.\n";
      welcome += "Ketik /led_off untuk mematikan LED.";
      bot.sendMessage(chat_id, welcome, "");
    }

    // Perintah untuk menyalakan LED
    if (text == "/led_on") {
      digitalWrite(ledPin, HIGH);
      bot.sendMessage(chat_id, "LED dinyalakan!", "");
    }

    // Perintah untuk mematikan LED
    if (text == "/led_off") {
      digitalWrite(ledPin, LOW);
      bot.sendMessage(chat_id, "LED dimatikan!", "");
    }

    // Respon default jika perintah tidak dikenali
    if (text != "/start" && text != "/led_on" && text != "/led_off") {
      bot.sendMessage(chat_id, "Perintah tidak dikenali!", "");
    }
  }
}

void setup() {
  Serial.begin(115200);

  // Konfigurasi pin LED sebagai output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Menghubungkan ke WiFi
  connectWiFi();

  // Mengatur client untuk koneksi yang aman
  client.setInsecure(); // Untuk melewati sertifikat SSL (tidak disarankan di aplikasi nyata)
}

void loop() {
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
  } else {
    // Jika WiFi terputus, coba hubungkan kembali
    connectWiFi();
  }
}
