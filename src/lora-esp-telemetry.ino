#include <SPI.h>
#include <LoRa.h>
#include <AESLib.h>

// LoRa SX1278 Pin Configuration for ESP8266
#define SS_PIN    15   // GPIO15 - NSS/CS
#define RST_PIN   16   // GPIO16 - Reset
#define DIO0_PIN  5    // GPIO5  - DIO0/IRQ

// LoRa Configuration
#define LORA_FREQUENCY    433E6  // 433 MHz
#define LORA_SYNC_WORD    0x12
#define LORA_SPREADING_FACTOR 7
#define LORA_BANDWIDTH    125E3
#define LORA_TX_POWER     17

// AES Configuration
#define AES_KEY_SIZE 16  // 128-bit key

// Global Variables
String inputBuffer = "";
uint16_t messageCounter = 0;
AESLib aesLib;

// AES Key (Change this to your own secret key!)
uint8_t aes_key[AES_KEY_SIZE] = {
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
  0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

// IV for AES (should be random for each message in production)
uint8_t aes_iv[16] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
};

void setup() {
  Serial.begin(9600);
  delay(1000);
  
  Serial.println("ESP8266 LoRa Interface with AES Encryption Starting...");
  
  // Initialize LoRa
  LoRa.setPins(SS_PIN, RST_PIN, DIO0_PIN);
  
  if (!LoRa.begin(LORA_FREQUENCY)) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  
  // Configure LoRa
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setSignalBandwidth(LORA_BANDWIDTH);
  LoRa.setSyncWord(LORA_SYNC_WORD);
  LoRa.setTxPower(LORA_TX_POWER);
  LoRa.enableCrc();
  
  // Initialize AES
  aesLib.gen_iv(aes_iv);
  
  Serial.println("LoRa Ready with AES Encryption!");
}

void loop() {
  // Handle incoming serial data (from UART or debug)
  handleSerialData();
  
  // Handle incoming LoRa packets
  handleLoRaReceive();
  
  delay(10);
}

void handleSerialData() {
  while (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      if (inputBuffer.length() > 0) {
        sendEncryptedLoRaMessage(inputBuffer);
        inputBuffer = "";
      }
    } else {
      inputBuffer += c;
      
      // Prevent buffer overflow
      if (inputBuffer.length() > 200) {
        inputBuffer = "";
      }
    }
  }
}

void sendEncryptedLoRaMessage(String message) {
  // Create packet with counter
  String packet = String(messageCounter) + ":" + message;
  
  // Encrypt the packet
  String encryptedPacket = encryptString(packet);
  
  if (encryptedPacket.length() > 0) {
    // Send encrypted packet
    LoRa.beginPacket();
    LoRa.print(encryptedPacket);
    LoRa.endPacket();
    
    Serial.println("TX: " + packet + " (Encrypted)");
    messageCounter++;
  } else {
    Serial.println("Encryption failed!");
  }
}

void handleLoRaReceive() {
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    String receivedMessage = "";
    
    // Read the packet
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }
    
    // Get signal strength
    int rssi = LoRa.packetRssi();
    
    // Decrypt the message
    String decryptedMessage = decryptString(receivedMessage);
    
    if (decryptedMessage.length() > 0) {
      Serial.println("RX: " + decryptedMessage + " (RSSI: " + String(rssi) + ")");
    } else {
      Serial.println("RX: Failed to decrypt message (RSSI: " + String(rssi) + ")");
    }
  }
}

String encryptString(String plaintext) {
  // Pad string to multiple of 16 bytes
  int len = plaintext.length();
  int paddedLen = ((len / 16) + 1) * 16;
  
  // Create buffer for padded data
  char input[paddedLen + 1];
  strcpy(input, plaintext.c_str());
  
  // Add PKCS7 padding
  int padValue = paddedLen - len;
  for (int i = len; i < paddedLen; i++) {
    input[i] = padValue;
  }
  input[paddedLen] = '\0';
  
  // Encrypt
  uint8_t encrypted[paddedLen];
  aesLib.encrypt((uint8_t*)input, paddedLen, encrypted, aes_key, sizeof(aes_key), aes_iv);
  
  // Convert to hex string
  String result = "";
  for (int i = 0; i < paddedLen; i++) {
    if (encrypted[i] < 16) result += "0";
    result += String(encrypted[i], HEX);
  }
  
  return result;
}

String decryptString(String ciphertext) {
  // Convert hex string to bytes
  int len = ciphertext.length() / 2;
  if (len == 0 || ciphertext.length() % 2 != 0) {
    return "";  // Invalid hex string
  }
  
  uint8_t encrypted[len];
  for (int i = 0; i < len; i++) {
    String hexByte = ciphertext.substring(i * 2, i * 2 + 2);
    encrypted[i] = (uint8_t)strtol(hexByte.c_str(), NULL, 16);
  }
  
  // Decrypt
  uint8_t decrypted[len];
  aesLib.decrypt(encrypted, len, decrypted, aes_key, sizeof(aes_key), aes_iv);
  
  // Remove PKCS7 padding
  int padValue = decrypted[len - 1];
  if (padValue > 0 && padValue <= 16) {
    // Verify padding
    bool validPadding = true;
    for (int i = len - padValue; i < len; i++) {
      if (decrypted[i] != padValue) {
        validPadding = false;
        break;
      }
    }
    
    if (validPadding) {
      decrypted[len - padValue] = '\0';
      return String((char*)decrypted);
    }
  }
  
  return "";  // Invalid padding or decryption failed
}

// Alternative simple XOR encryption (lighter weight)
String xorEncrypt(String data, String key) {
  String result = "";
  for (int i = 0; i < data.length(); i++) {
    result += (char)(data[i] ^ key[i % key.length()]);
  }
  return result;
}

String xorDecrypt(String data, String key) {
  return xorEncrypt(data, key);  // XOR is symmetric
}
