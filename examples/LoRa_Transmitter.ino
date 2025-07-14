#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Transmitter");

  LoRa.setPins(D8, D0, D1); // NSS, RESET, DIO0

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  Serial.println("Sending message...");

  LoRa.beginPacket();
  LoRa.print("Hello from ESP8266!");
  LoRa.endPacket();

  delay(2000); // Send every 2 seconds
}
