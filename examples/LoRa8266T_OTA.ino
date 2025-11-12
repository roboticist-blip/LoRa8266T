// With the above code you can just update the code on esp8266 over the air

#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <SPI.h>
#include <LoRa.h>

const char* ssid = "ssid";
const char* password = "password";

void setupOTA(const char* hostname, const char* password) {
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword(password);
  ArduinoOTA.begin();
}
void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void setup() {
  Serial.begin(115200);
  setupWiFi();
  setupOTA("ESP8266OTA", "8266");
  pinMode(2, OUTPUT);  
  Serial.println("LoRa Transmitter");
  LoRa.setPins(5, 16, 4); // NSS, RESET, DIO0
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  ArduinoOTA.handle(); 
  Serial.println("Sending message...");
  LoRa.beginPacket();
  LoRa.print("Hello from ESP8266!");
  LoRa.endPacket();
  ArduinoOTA.handle(); 

}
