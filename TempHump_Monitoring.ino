#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define DHTPIN D4        // Pin tempat sensor DHT22 terhubung
#define DHTTYPE DHT22    // Tipe sensor DHT22

char auth[] = "mnBmj5aj0LEKUdghiHnG81HKlt1aDFMR";  // Token autentikasi Blynk
char ssid[] = "Kosan bu nata";                     // Nama WiFi Anda
char pass[] = "immodium";                           // Password WiFi Anda

#define BLYNK_TEMPLATE_ID "TMPLxyk260wi"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"

DHT dht(DHTPIN, DHTTYPE);
bool readSensor = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
}

void loop() {
  Blynk.run();
  if (readSensor) {
    sendSensorData();
    readSensor = false; // Setel kembali ke false setelah membaca dan mengirim data
  }
}

void sendSensorData() {
  float h = dht.readHumidity();      // Membaca kelembapan udara
  float t = dht.readTemperature();   // Membaca suhu dalam Celcius
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    Blynk.virtualWrite(V2, "Failed to read from DHT sensor!"); // Kirim pesan ke pin V2 di Blynk
    delay(2000);
    Blynk.virtualWrite(V2, "Refresh to Update!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *C");

  // Mengirim data ke Blynk
  Blynk.virtualWrite(V1, h);  // Mengirim data kelembapan ke pin V5 di Blynk
  Blynk.virtualWrite(V0, t);  // Mengirim data suhu ke pin V6 di Blynk
  
  Blynk.virtualWrite(V2, "Data Received!"); // Kirim pesan ke pin V2 di Blynk bahwa data berhasil diterima
  delay(3000);
  Blynk.virtualWrite(V2, "Refresh to Update!");
}

// Callback yang dipanggil saat tombol di Blynk ditekan
BLYNK_WRITE(V3) {
  int buttonState = param.asInt(); // Membaca status tombol (1 = ditekan, 0 = dilepas)
  if (buttonState == 1) {
    readSensor = true; // Setel flag untuk membaca sensor
  }
}
