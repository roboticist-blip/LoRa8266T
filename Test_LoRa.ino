#include <Arduino.h>
#include "LoRa8266T.h"

void testEncryption() {
    String original = "Hello";
    String encrypted = encryptAES(original, "mykey123");
    String decrypted = decryptAES(encrypted, "mykey123");

    if (original == decrypted) {
        Serial.println("PASS: Encryption works!");
    } else {
        Serial.println("FAIL: Something went wrong.");
    }
}
