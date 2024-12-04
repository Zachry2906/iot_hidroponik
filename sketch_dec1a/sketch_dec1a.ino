#include <WiFi.h>
#include <HTTPClient.h>

// Konfigurasi WiFi
const char* ssid = "upil";
const char* password = "11111111";

// Konfigurasi server
const char* serverName = "http://172.20.10.2/watermonitoring/insert_data.php";

// Pin definisi
#define WATER_LEVEL_PIN 35    // Pin sensor water liquid level depth
#define TRIG_PIN 32           // Pin trig untuk ultrasonic sensor
#define ECHO_PIN 33           // Pin echo untuk ultrasonic sensor
#define RELAY_PIN 34          // Pin untuk relay

// Variabel untuk water level sensor
int waterLevelValue = 0;

// Variabel untuk ultrasonic sensor
long duration;
float distance;
float volume;

// Konfigurasi tangki
const float tankHeight = 15.0;   // Tinggi tangki dalam cm
const float maxVolume = 1400.0; // Volume maksimum dalam mL

int relay1State = LOW;

void setup() {
  Serial.begin(115200);
    digitalWrite(RELAY_PIN, relay1State);
  // Konfigurasi pin
  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_PIN, OUTPUT);

  // Matikan relay saat awal
  digitalWrite(RELAY_PIN, LOW);

  // Koneksi WiFi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");

  
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("ESP32 Water Monitoring Debug");
  Serial.println("===========================");
}

void loop() {
  // Baca nilai sensor water level
  waterLevelValue = analogRead(WATER_LEVEL_PIN);

  // Logika relay

  // Baca data dari sensor ultrasonik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Hitung durasi echo dan jarak
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration * 0.034) / 2;

  // Hitung volume air berdasarkan jarak
  float waterHeight = tankHeight - distance; // Tinggi air dalam cm
  if (waterHeight < 0) waterHeight = 0;      // Jika jarak lebih besar dari tinggi tangki
  volume = (waterHeight / tankHeight) * maxVolume;

  // Tampilkan data di Serial Monitor
  Serial.println("--- Water Monitoring Data ---");
  Serial.print("Water Level (Raw Value): ");
  Serial.println(waterLevelValue);


  Serial.print("Distance (cm): ");
  Serial.println(distance);

  Serial.print("Water Volume (mL): ");
  Serial.println(volume);

  // Kirim data ke server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Persiapkan URL dan konten
    http.begin(serverName);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Siapkan string data
    String httpRequestData = "water_level=" + String(waterLevelValue) + 
                             "&distance=" + String(distance) + 
                             "&volume=" + String(volume);
    
    // Kirim HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    
    // Cetak kode respon
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    
    // Tutup koneksi
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  // Delay untuk stabilitas dan menghindari flooding server
  delay(5000); // Kirim data setiap 5 detik
}
