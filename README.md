ESP8266 LoRa Encrypted Communication

A secure wireless communication system using ESP8266 microcontrollers and LoRa SX1278 modules with AES encryption.
Features

    Long Range Communication: LoRa 433MHz for extended range
    AES-128 Encryption: Secure message transmission with PKCS7 padding
    Message Counter: Built-in message sequencing
    RSSI Monitoring: Signal strength indication
    Serial Interface: Easy debugging and message input
    Fallback XOR Encryption: Lightweight alternative encryption method

Hardware Requirements
Components

    ESP8266 development board (NodeMCU, Wemos D1 Mini, etc.)
    LoRa SX1278 module (433MHz)
    Jumper wires
    Breadboard (optional)

Wiring Diagram
LoRa SX1278 Wiring Charts
ESP8266 (NodeMCU/Wemos D1 Mini)
ESP8266 GPIO 	SX1278 Pin 	Description
GPIO15 	NSS/CS 	Chip Select
GPIO16 	RST 	Reset
GPIO5 	DIO0 	Interrupt
3.3V 	VCC 	Power
GND 	GND 	Ground
GPIO14 	SCK 	SPI Clock
GPIO13 	MOSI 	SPI Data Out
GPIO12 	MISO 	SPI Data In
ESP32 (DevKit/WROOM)
ESP32 GPIO 	SX1278 Pin 	Description
GPIO5 	NSS/CS 	Chip Select
GPIO14 	RST 	Reset
GPIO2 	DIO0 	Interrupt
3.3V 	VCC 	Power
GND 	GND 	Ground
GPIO18 	SCK 	SPI Clock
GPIO23 	MOSI 	SPI Data Out
GPIO19 	MISO 	SPI Data In
ESP32 Alternative Pin Configuration
ESP32 GPIO 	SX1278 Pin 	Description
GPIO15 	NSS/CS 	Chip Select
GPIO4 	RST 	Reset
GPIO26 	DIO0 	Interrupt
3.3V 	VCC 	Power
GND 	GND 	Ground
GPIO18 	SCK 	SPI Clock
GPIO23 	MOSI 	SPI Data Out
GPIO19 	MISO 	SPI Data In
Arduino Nano
Arduino Nano Pin 	SX1278 Pin 	Description
D10 	NSS/CS 	Chip Select
D9 	RST 	Reset
D2 	DIO0 	Interrupt
3.3V 	VCC 	Power
GND 	GND 	Ground
D13 	SCK 	SPI Clock
D11 	MOSI 	SPI Data Out
D12 	MISO 	SPI Data In
Arduino Nano Pin Reference

    D10 = Digital Pin 10
    D9 = Digital Pin 9
    D2 = Digital Pin 2 (Interrupt capable)
    D13 = Digital Pin 13 (Built-in LED pin)
    D11 = Digital Pin 11
    D12 = Digital Pin 12

Arduino Uno
Arduino Uno Pin 	SX1278 Pin 	Description
D10 	NSS/CS 	Chip Select
D9 	RST 	Reset
D2 	DIO0 	Interrupt
3.3V 	VCC 	Power
GND 	GND 	Ground
D13 	SCK 	SPI Clock
D11 	MOSI 	SPI Data Out
D12 	MISO 	SPI Data In
Arduino Uno Pin Reference

    D10 = Digital Pin 10 (SS - Slave Select)
    D9 = Digital Pin 9
    D2 = Digital Pin 2 (External Interrupt 0)
    D13 = Digital Pin 13 (SCK + Built-in LED)
    D11 = Digital Pin 11 (MOSI)
    D12 = Digital Pin 12 (MISO)

Code Pin Definitions
ESP32 Configuration

// LoRa SX1278 Pin Configuration for ESP32
#define SS_PIN    5    // GPIO5 - NSS/CS
#define RST_PIN   14   // GPIO14 - Reset
#define DIO0_PIN  2    // GPIO2 - DIO0/IRQ

// Alternative ESP32 Configuration
#define SS_PIN    15   // GPIO15 - NSS/CS
#define RST_PIN   4    // GPIO4 - Reset
#define DIO0_PIN  26   // GPIO26 - DIO0/IRQ

Arduino Nano Configuration

// LoRa SX1278 Pin Configuration for Arduino Nano
#define SS_PIN    10   // D10 - NSS/CS
#define RST_PIN   9    // D9 - Reset
#define DIO0_PIN  2    // D2 - DIO0/IRQ (Interrupt)

Arduino Uno Configuration

// LoRa SX1278 Pin Configuration for Arduino Uno
#define SS_PIN    10   // D10 - NSS/CS
#define RST_PIN   9    // D9 - Reset
#define DIO0_PIN  2    // D2 - DIO0/IRQ (Interrupt)

Important Notes
Power Supply Requirements

    ESP8266/ESP32: Native 3.3V operation ✅
    Arduino Nano/Uno: 5V boards - Use 3.3V output pin for LoRa module
    LoRa modules are 3.3V devices - don't connect to 5V!

SPI Bus Sharing

All microcontrollers use hardware SPI:

    SCK: SPI Clock (shared)
    MOSI: Master Out Slave In (shared)
    MISO: Master In Slave Out (shared)
    SS/CS: Slave Select (unique per device)

Interrupt Pins

    ESP8266: Any GPIO can be interrupt
    ESP32: Any GPIO can be interrupt
    Arduino Nano/Uno: Only pins 2 and 3 support external interrupts

Pin Change Options
For Arduino Nano/Uno - Alternative Pins:

// Alternative configuration if pins are occupied
#define SS_PIN    8    // D8 - NSS/CS
#define RST_PIN   7    // D7 - Reset  
#define DIO0_PIN  3    // D3 - DIO0/IRQ (Interrupt pin)

For ESP32 - Using VSPI:

// Using VSPI pins (default)
// SCK = GPIO18, MISO = GPIO19, MOSI = GPIO23

// Using HSPI pins (alternative)
// SCK = GPIO14, MISO = GPIO12, MOSI = GPIO13
// Change SS, RST, DIO0 accordingly

Software Requirements
Arduino IDE Setup

    Install ESP8266 board package in Arduino IDE
    Install required libraries:
        LoRa by Sandeep Mistry
        AESLib by DavyLandman

Library Installation

Arduino IDE → Sketch → Include Library → Manage Libraries
Search and install:
- LoRa
- AESLib

Configuration
LoRa Settings

#define LORA_FREQUENCY    433E6  // 433 MHz
#define LORA_SYNC_WORD    0x12
#define LORA_SPREADING_FACTOR 7
#define LORA_BANDWIDTH    125E3
#define LORA_TX_POWER     17

Security Configuration

⚠️ IMPORTANT: Change the default AES key before deployment!

uint8_t aes_key[AES_KEY_SIZE] = {
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
  0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};

Usage

    Upload the code to two or more ESP8266 devices
    Open Serial Monitor at 9600 baud rate
    Type messages in the serial monitor and press Enter
    Messages are automatically encrypted and transmitted via LoRa
    Received messages are decrypted and displayed with RSSI values

Example Output

ESP8266 LoRa Interface with AES Encryption Starting...
LoRa Ready with AES Encryption!
TX: 0:Hello World (Encrypted)
RX: 1:Hello back! (RSSI: -45)

Security Features

    AES-128 CBC Encryption: Industry-standard encryption
    PKCS7 Padding: Proper block cipher padding
    Message Counter: Prevents replay attacks
    IV Generation: Initialization vector for encryption
    XOR Fallback: Lightweight encryption option

Customization
Changing Frequency

Modify LORA_FREQUENCY to match your regional ISM band:

    433MHz (Europe, Asia)
    915MHz (North America)
    868MHz (Europe)

Adjusting Range vs Speed

    Longer Range: Increase spreading factor (7-12)
    Faster Speed: Decrease spreading factor, increase bandwidth

Troubleshooting
Common Issues

    LoRa init failed: Check wiring connections
    No messages received: Verify frequency and sync word match
    Decryption failed: Ensure both devices use the same AES key
    Poor range: Check antenna connection and TX power settings

Debug Tips

    Monitor serial output for TX/RX confirmations
    Check RSSI values for signal quality
    Verify LoRa module power supply (3.3V, sufficient current)

Performance

    Range: Up to 2km line-of-sight (depends on environment)
    Data Rate: ~5.5kbps at SF7
    Power Consumption: ~100mA during transmission

Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.
Development Setup

    Fork the repository
    Create your feature branch (git checkout -b feature/amazing-feature)
    Commit your changes (git commit -m 'Add amazing feature')
    Push to the branch (git push origin feature/amazing-feature)
    Open a Pull Request

License

This project is licensed under the MIT License - see the LICENSE file for details.
Acknowledgments

    LoRa library by Sandeep Mistry
    AESLib by DavyLandman
    ESP8266 Community

Disclaimer

This project is for educational and experimental purposes. Ensure compliance with local radio regulations when using LoRa frequencies. The default AES key is for demonstration only - always use your own secure key in production.

⭐ Star this repository if you find it useful!
