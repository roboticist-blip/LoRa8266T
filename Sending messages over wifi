/* How to connect:
  1. Upload the program in both of your reciever and transmitter (both will work as transciever) esp8266-loRa device.
  2. After uploading open serial monitor (9600 braudrate) and copy the IP address.
  3. Here the ESP8266 is in acces point mode so to connect to it, Turn your wifi on and here you will see a device named as LoRa-Gateway-1 (Or as you defined).
  4. Connect to this device by entering the password password (12345678 default or you can change in the code).
  5. Then open your browser, Enter the IP and you are ready to transfer the data.

    Although I am working to direclty select the sensor through the website and share the data soon i will update this repo by that updates.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <LoRa.h>
#include <AESLib.h>

// WiFi Access Point Configuration
const char* ap_ssid = "LoRa-Gateway-1";
const char* ap_password = "12345678";  // Change this password!

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
ESP8266WebServer server(80);

// Store recent messages for web display
String recentMessages[10];
int messageIndex = 0;

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
  
  Serial.println("ESP8266 LoRa WiFi Gateway Starting...");
  
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
  
  // Setup WiFi Access Point
  setupWiFiAP();
  
  // Setup Web Server
  setupWebServer();
  
  Serial.println("System Ready!");
  Serial.print("Connect to WiFi: ");
  Serial.println(ap_ssid);
  Serial.print("Password: ");
  Serial.println(ap_password);
  Serial.println("Open browser to: http://192.168.4.1");
}

void loop() {
  // Handle web server clients
  server.handleClient();
  
  // Handle incoming serial data (still available for debugging)
  handleSerialData();
  
  // Handle incoming LoRa packets
  handleLoRaReceive();
  
  delay(10);
}

void setupWiFiAP() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
}

void setupWebServer() {
  // Main page
  server.on("/", handleRoot);
  
  // Send message endpoint
  server.on("/send", HTTP_POST, handleSendMessage);
  
  // Get messages endpoint (for AJAX)
  server.on("/messages", HTTP_GET, handleGetMessages);
  
  // API endpoint for sending messages (alternative)
  server.on("/api/send", HTTP_POST, handleAPISend);
  
  server.begin();
  Serial.println("Web server started");
}

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>LoRa Gateway</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  html += "<style>";
  html += "body { font-family: Arial; margin: 20px; background: #f0f0f0; }";
  html += ".container { max-width: 600px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }";
  html += "h1 { color: #333; text-align: center; }";
  html += ".input-group { margin: 20px 0; }";
  html += "label { display: block; margin-bottom: 5px; font-weight: bold; }";
  html += "input[type=\"text\"] { width: 100%; padding: 10px; border: 1px solid #ddd; border-radius: 5px; font-size: 16px; }";
  html += "button { background: #007bff; color: white; padding: 12px 24px; border: none; border-radius: 5px; cursor: pointer; font-size: 16px; }";
  html += "button:hover { background: #0056b3; }";
  html += ".messages { margin-top: 30px; }";
  html += ".message { background: #f8f9fa; padding: 10px; margin: 5px 0; border-left: 4px solid #007bff; border-radius: 3px; }";
  html += ".message.received { border-left-color: #28a745; }";
  html += ".status { margin: 10px 0; padding: 10px; border-radius: 5px; }";
  html += ".success { background: #d4edda; color: #155724; border: 1px solid #c3e6cb; }";
  html += ".error { background: #f8d7da; color: #721c24; border: 1px solid #f5c6cb; }";
  html += "</style></head><body>";
  html += "<div class=\"container\">";
  html += "<h1>LoRa Gateway Control</h1>";
  html += "<div class=\"input-group\">";
  html += "<label for=\"message\">Message to Send:</label>";
  html += "<input type=\"text\" id=\"message\" placeholder=\"Enter your message here...\" maxlength=\"200\">";
  html += "<br><br><button onclick=\"sendMessage()\">Send via LoRa</button>";
  html += "</div>";
  html += "<div id=\"status\"></div>";
  html += "<div class=\"messages\">";
  html += "<h3>Recent Messages:</h3>";
  html += "<div id=\"messageList\"><p>No messages yet...</p></div>";
  html += "</div></div>";
  
  // JavaScript
  html += "<script>";
  html += "function sendMessage() {";
  html += "const message = document.getElementById('message').value;";
  html += "if (!message.trim()) { showStatus('Please enter a message', 'error'); return; }";
  html += "fetch('/send', { method: 'POST', headers: { 'Content-Type': 'application/x-www-form-urlencoded' }, body: 'message=' + encodeURIComponent(message) })";
  html += ".then(response => response.text())";
  html += ".then(data => { if (data.includes('sent')) { showStatus('Message sent successfully!', 'success'); document.getElementById('message').value = ''; } else { showStatus('Failed to send message', 'error'); } })";
  html += ".catch(error => { showStatus('Error sending message', 'error'); }); }";
  html += "function showStatus(message, type) { const status = document.getElementById('status'); status.innerHTML = '<div class=\"status ' + type + '\">' + message + '</div>'; setTimeout(() => status.innerHTML = '', 3000); }";
  html += "function updateMessages() { fetch('/messages').then(response => response.json()).then(data => { const messageList = document.getElementById('messageList'); if (data.messages && data.messages.length > 0) { messageList.innerHTML = data.messages.map(msg => '<div class=\"message ' + (msg.type || '') + '\">' + msg.text + '</div>').join(''); } else { messageList.innerHTML = '<p>No messages yet...</p>'; } }); }";
  html += "document.getElementById('message').addEventListener('keypress', function(e) { if (e.key === 'Enter') { sendMessage(); } });";
  html += "setInterval(updateMessages, 2000); updateMessages();";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

void handleSendMessage() {
  if (server.hasArg("message")) {
    String message = server.arg("message");
    sendEncryptedLoRaMessage(message);
    server.send(200, "text/plain", "Message sent via LoRa");
  } else {
    server.send(400, "text/plain", "No message provided");
  }
}

void handleGetMessages() {
  String json = "{\"messages\":[";
  
  for (int i = 0; i < 10; i++) {
    if (recentMessages[i].length() > 0) {
      if (json.length() > 15) json += ",";
      json += "{\"text\":\"" + recentMessages[i] + "\",\"type\":\"";
      json += (recentMessages[i].startsWith("TX:")) ? "sent" : "received";
      json += "\"}";
    }
  }
  
  json += "]}";
  server.send(200, "application/json", json);
}

void handleAPISend() {
  if (server.hasArg("plain")) {
    String message = server.arg("plain");
    sendEncryptedLoRaMessage(message);
    server.send(200, "application/json", "{\"status\":\"sent\",\"message\":\"" + message + "\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"No data provided\"}");
  }
}

void addToRecentMessages(String message) {
  recentMessages[messageIndex] = message;
  messageIndex = (messageIndex + 1) % 10;
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
    
    String logMessage = "TX: " + packet + " (Encrypted)";
    Serial.println(logMessage);
    addToRecentMessages(logMessage);
    messageCounter++;
  } else {
    Serial.println("Encryption failed!");
    addToRecentMessages("TX: Encryption failed!");
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
    
    String logMessage;
    if (decryptedMessage.length() > 0) {
      logMessage = "RX: " + decryptedMessage + " (RSSI: " + String(rssi) + ")";
    } else {
      logMessage = "RX: Failed to decrypt message (RSSI: " + String(rssi) + ")";
    }
    
    Serial.println(logMessage);
    addToRecentMessages(logMessage);
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
