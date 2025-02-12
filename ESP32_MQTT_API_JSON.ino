#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <ArduinoJson.h>

//OLED
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//MIC_PIN
#define MIC_PIN 4  // ADC1 channel
const int sampleSize = 1024;  // Adjust based on needs
float amplitude[sampleSize];
float timeValues[sampleSize];


// MQTT Broker details
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* publish_topic = "test/audio/1";

const char* ssid = "TEST";
const char* password = "1234";

// Wi-Fi and MQTT clients
WiFiClient espClient;
PubSubClient client(espClient);

void setupWiFi() {
  delay(10);
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("WIFI:TEST");
  display.setCursor(0, 40);
  display.println("PASS:1234");
  display.display();
  delay(2000);
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

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      client.publish(publish_topic, "hello world");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void setup() {
  Serial.begin(115200);
  setupWiFi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


  for (int i = 0; i < sampleSize; i++) {
        timeValues[i] = i * 0.01; // Example: 10ms interval
    }
}

void loop() {
      for (int i = 0; i < sampleSize; i++) {
        int rawValue = analogRead(MIC_PIN);
        amplitude[i] = (rawValue / 4095.0) * 100.0; // Convert to range 0-100
        delayMicroseconds(62);  // ~16kHz sample rate
    }

    // Create JSON
    StaticJsonDocument<2048> jsonDoc;
    JsonArray amplitudeArray = jsonDoc.createNestedArray("amplitude");
    JsonArray timeArray = jsonDoc.createNestedArray("time");

    for (int i = 0; i < sampleSize; i++) {
        amplitudeArray.add(amplitude[i]);
        timeArray.add(timeValues[i]);
    }

    // Serialize and print JSON
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    Serial.println(jsonString);
  
    // Example of publishing a message
    static unsigned long lastTime = 0;
    if (millis() - lastTime > 1000) {
      lastTime = millis();

      lastTime = millis();

      client.publish(publish_topic, jsonString.c_str());
      Serial.println("Message published.");
         
      display.setTextSize(1);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(0, 0);
      display.println("TEST");
      display.setCursor(0, 40);
      display.print("IP : ");
      display.println(WiFi.localIP()); // Print the ESP32's IP address
      display.display();
      delay(2000);
      display.clearDisplay();
      
  delay(100);  // Adjust based on transmission needs
  }
}


