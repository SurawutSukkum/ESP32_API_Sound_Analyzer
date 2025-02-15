#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>


#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO


Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



//MIC_PIN
#define MIC_PIN 4  // ADC1 channel


    
// Wi-Fi credentials
const char* ssid = "TEST";
const char* password = "1234";
const char* serverUrl = "https://esp32-api-sound-analyzer.onrender.com/receive_data"; // Replace with your server

// MQTT Broker details
String numbers[256];
const int numSamples = 2000;
float amplitude[numSamples];
float timeValues[numSamples];



void setupWiFi() {
  delay(10);
  Serial.println("Connecting to Wi-Fi...");
  //WiFi.begin(ssid, password);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi.");
    // Once connected, print IP address
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); // Print the ESP32's IP address
}

void setup() {
  Serial.begin(115200);
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("Electronic ");
  display.setCursor(0, 10);
  display.println("stethoscope");
  display.display();

  delay(2000);


  
  setupWiFi();

    for (int i = 0; i < numSamples; i++) {
        timeValues[i] = i * 0.01;  // Example: 10ms interval
    }
}

void publishSensorData() {
  
    for (int i = 0; i < numSamples; i++) {
        int rawValue = analogRead(MIC_PIN);
        amplitude[i] = (rawValue / 4095.0) * 100.0; // Convert to 0-100 range
        delay(1); // 1ms delay to achieve 1000 samples in 1 second
    }

    // Create JSON
    StaticJsonDocument<8192> jsonDoc;
    JsonArray amplitudeArray = jsonDoc.createNestedArray("amplitude");
    JsonArray timeArray = jsonDoc.createNestedArray("time");

    for (int i = 0; i < numSamples; i++) {
        amplitudeArray.add(amplitude[i]);
        timeArray.add(timeValues[i]);
    }

    // Serialize JSON
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        String jsonPayload = String(jsonString.c_str()) ;
        
        int httpResponseCode = http.POST(jsonPayload);
        display.setTextSize(1);
        display.setTextColor(SH110X_WHITE);
        display.setCursor(0, 0);
        display.println("Electronic ");
        display.setCursor(0, 10);
        display.println("stethoscope");
        display.setCursor(0, 20);
        display.println("API , ");
        display.setCursor(0, 30);
        display.println(String(httpResponseCode));
        display.setCursor(0, 40);
        display.println(    WiFi.localIP());
    
        display.display();
        delay(2000);
        display.clearDisplay();
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Server Response: " + response);
        } else {
            Serial.print("Error in POST request: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
}
void api()
{
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
void sendSineWaveData(float amplitude) {
  
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(serverUrl);
        http.addHeader("Content-Type", "application/json");

        String jsonPayload = "{\"amplitude\": " + String(amplitude) + "}";
        
        int httpResponseCode = http.POST(jsonPayload);
        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println("Server Response: " + response);
        } else {
            Serial.print("Error in POST request: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.println("WiFi Disconnected!");
    }
}
void loop() {
   
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 1000) {
    publishSensorData();
    lastTime = millis();
 
    

  }
}
