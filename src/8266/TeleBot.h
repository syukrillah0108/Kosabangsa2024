#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define BOT_TOKEN "7679786493:AAGuYqA8evAvANNHvMIydReW0aPGWP4ICSQ" // Ganti dengan Bot Token kamu
WiFiClientSecure client_bot;
UniversalTelegramBot bot(BOT_TOKEN, client_bot);

// Variabel untuk memeriksa update bot
unsigned long lastTimeBotRan;
const long botInterval = 500; // Interval 1 detik
String msg;
String chat_id, text_msg, from_name;
unsigned long previousMillis = 0; // Variabel untuk menyimpan waktu sebelumnya
const long interval = 10000;      // Interval 10 detik (dalam milidetik)

#define CHATID1 "6762920775"
#define CHATID2 "1312129748"

void handleNewMessages(int numNewMessages){
    Serial.println("Pesan baru diterima:");

    for (int i = 0; i < numNewMessages; i++) {
        chat_id = bot.messages[i].chat_id;
        text_msg = bot.messages[i].text;
        from_name = bot.messages[i].from_name;
        if(chat_id == CHATID1 || chat_id == CHATID2){

            if (from_name == "") from_name = "User";

            Serial.print("Pesan dari ");
            Serial.print(from_name);
            Serial.print(": ");
            Serial.println(text_msg);

            if (text_msg == "/start") {
                msg = "Selamat Datang, " + from_name + "!\n\n";
                msg += "Ketik /kondisi : Kondisi Rumah Pengering.\n";
                bot.sendMessage(chat_id, msg, "");
                Serial.println(from_name);
                Serial.println(chat_id);
            }else if (text_msg == "/kondisi") {
                msg += "Umum\n";
                msg += "    Suhu: ";
                msg += String(Temp1);
                msg += " ℃\n";
                msg += "    Lempap: ";
                msg += String(Hum1);
                msg += " %\n";

                msg += "\nNori\n";
                msg += "    Suhu: ";
                msg += String(Temp3);
                msg += " ℃\n";
                msg += "    Lempap: ";
                msg += String(Hum3);
                msg += " %\n";

                msg += "\n\nKetik /start untuk membuka menu.";
                bot.sendMessage(chat_id, msg, "");
            }
        }
        msg.clear();
    }
}
