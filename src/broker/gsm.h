#include <HardwareSerial.h>

HardwareSerial sim800(1);

// Daftar nomor yang diizinkan (ganti dengan nomor Anda)
const String allowedNumbers[] = {"+6285774244205", "+6285880216919", "+6289876543210"};
const int allowedNumbersCount = sizeof(allowedNumbers) / sizeof(allowedNumbers[0]);

String senderNumber = "";  // Variabel untuk menyimpan nomor pengirim
String message = "";       // Variabel untuk menyimpan isi pesan

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
}
void loop(){
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

        replySMS(senderNumber, "Terima kasih, pesan Anda telah diterima!");
      // Cek apakah nomor pengirim diizinkan
      if (isAllowedNumber(senderNumber)) {
        Serial.println("Authorized sender. Responding...");
        delay(1000);
      } else {
        Serial.println("Unauthorized sender. Ignoring message.");
      }
    }
  }
}