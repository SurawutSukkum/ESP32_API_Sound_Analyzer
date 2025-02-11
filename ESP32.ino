#include <WiFi.h>
#include <HTTPClient.h>

#define MIC_PIN 4  // ADC1 channel

const char* ssid = "YourSSID";
const char* password = "YourPassword";
const char* serverUrl = "http://127.0.0.1:5000/audio"; // Replace with your server command cmd ipconfig/all

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the ESP32's IP address
}

void loop() {
  const int sampleSize = 1024;  // Adjust based on needs
  uint16_t audioData[sampleSize];

  // Read ADC values
  for (int i = 0; i < sampleSize; i++) {
    audioData[i] = analogRead(MIC_PIN);
    delayMicroseconds(62);  // ~16kHz sample rate
  }

  // Send data to server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/octet-stream");

    int httpResponseCode = http.POST((uint8_t*)audioData, sizeof(audioData));
    Serial.println("Response: " + String(httpResponseCode));
    http.end();
  }

  delay(100);  // Adjust based on transmission needs
}
