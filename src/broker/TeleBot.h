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
int kalibrasi = 70;
String valuePart;

bool isNumeric(String str) {
  for (unsigned int i = 0; i < str.length(); i++) {
    if (!isDigit(str[i])) {
      return false;
    }
  }
  return true;
}

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

                msg += "\nKalibrasi\n";
                msg += "    Relay: ";
                msg += String(kalibrasi);

                msg += "\n\nKetik /start untuk membuka menu.";
                msg += "\n\nKetik set:<nilai> untuk mengubah kalibrasi.";
                bot.sendMessage(chat_id, msg, "");
            }else if(text_msg.startsWith("set:")){
                valuePart = text_msg.substring(4);
                if(isNumeric(valuePart)){
                    kalibrasi = valuePart.toInt();
                    bot.sendMessage(chat_id, "Kalibrasi Berhasil di Ubah", "");
                }else{
                    bot.sendMessage(chat_id, "Format salah", "");
                }
            }
        }
        msg.clear();
    }
}
