#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define BOT_TOKEN "7679786493:AAGuYqA8evAvANNHvMIydReW0aPGWP4ICSQ" // Ganti dengan Bot Token kamu
WiFiClientSecure client_bot;
UniversalTelegramBot bot(BOT_TOKEN, client_bot);

// Variabel untuk memeriksa update bot
unsigned long lastTimeBotRan;
const long botInterval = 500; // Interval 1 detik
String msg;

unsigned long previousMillis = 0; // Variabel untuk menyimpan waktu sebelumnya
const long interval = 10000;      // Interval 10 detik (dalam milidetik)

#define CHATID "6762920775"//"1348547429"

void handleNewMessages(int numNewMessages){
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

        if (text == "/start") {
            msg = "Selamat Datang, " + from_name + "!\n\n";
            msg += "Ketik /kondisi : Kondisi Rumah Pengering.\n";
            bot.sendMessage(chat_id, msg, "");
            Serial.println(from_name);
            Serial.println(chat_id);
        }else if (text == "/kondisi") {
            msg += "Umum\n";
            msg += "    Suhu: 63 ℃\n";
            msg += "    Lempap: 23 %\n";

            msg += "\nNori\n";
            msg += "    Suhu: 71 ℃ ⚠️\n";
            msg += "    Lempap: 20 %\n";

            msg += "\n\nKetik /start untuk membuka menu.";
            bot.sendMessage(chat_id, msg, "");
        }
    }
    msg.clear();
}
