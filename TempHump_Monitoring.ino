#define BLYNK_TEMPLATE_ID "ADD_YOUR_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#include <TimeLib.h>    // Tambahkan library TimeLib
#include <WidgetRTC.h>  // Tambahkan library WidgetRTC untuk sinkronisasi waktu

#define DHTPIN D7      // Pin tempat sensor DHT22 terhubung
#define DHTTYPE DHT22  // Tipe sensor DHT22

char auth[] = "";  // Token autentikasi Blynk
char ssid[] = "";                     // Nama WiFi Anda
char pass[] = "";                          // Password WiFi Anda

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;  // Menggunakan BlynkTimer
WidgetRTC rtc;     // Objek untuk sinkronisasi waktu

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  rtc.begin();

  // Set timer untuk memanggil fungsi sendSensorData setiap 60 detik
  timer.setInterval(60000L, sendSensorData);
  setSyncInterval(10 * 60);  // Sinkronkan waktu setiap 10 menit
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendSensorData() {
  float h = dht.readHumidity();     // Membaca kelembapan udara
  float t = dht.readTemperature();  // Membaca suhu dalam Celcius

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    Blynk.virtualWrite(V2, "Failed to read from DHT sensor!");  // Kirim pesan ke pin V2 di Blynk
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  // Mengirim data ke Blynk
  Blynk.virtualWrite(V1, h);  // Mengirim data kelembapan ke pin V1 di Blynk
  Blynk.virtualWrite(V0, t);  // Mengirim data suhu ke pin V0 di Blynk

  Blynk.virtualWrite(V2, "Data Updated!");  // Kirim pesan ke pin V2 di Blynk bahwa data berhasil diterima
  delay(1000);
  Blynk.virtualWrite(V2, "Refresh to Update!");

  // Dapatkan waktu saat ini
  int hours = hour();
  int minutes = minute();

  // Format waktu menjadi hh:mm
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", hours, minutes);

  // Kirim waktu ke pin V4 di Blynk
  Blynk.virtualWrite(V4, timeStr);
}

// Callback yang dipanggil saat tombol di Blynk ditekan
BLYNK_WRITE(V3) {
  int buttonState = param.asInt();  // Membaca status tombol (1 = ditekan, 0 = dilepas)
  if (buttonState == 1) {
    sendSensorData();  // Langsung kirim data sensor saat tombol ditekan
  }
}

// Fungsi untuk sinkronisasi waktu
BLYNK_CONNECTED() {
  rtc.begin();
}
