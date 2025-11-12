# 📡 LoRa8266T - OTA Wireless Telemetry System

<div align="center">

![LoRa](https://img.shields.io/badge/LoRa-SX1278-blue?style=for-the-badge&logo=wifi&logoColor=white)
![ESP8266](https://img.shields.io/badge/ESP8266-Compatible-red?style=for-the-badge&logo=espressif&logoColor=white)
![AES](https://img.shields.io/badge/AES--128-Encrypted-green?style=for-the-badge&logo=security&logoColor=white)
![License](https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge)

**A secure, long-range wireless communication system using ESP8266 and LoRa SX1278 modules with military-grade AES-128 encryption**

[Features](#-features) • [Hardware](#-hardware-requirements) • [Installation](#-installation) • [Usage](#-usage) • [Documentation](#-documentation)

</div>

---

## 🌟 Features

<table>
<tr>
<td width="50%">

### 🚀 **Performance**
- **Long Range Communication**: Up to 2km line-of-sight using LoRa 433MHz
- **Fast Data Rate**: ~5.5kbps at SF7
- **Low Latency**: Real-time message transmission
- **Signal Monitoring**: Built-in RSSI feedback

</td>
<td width="50%">

### 🔒 **Security**
- **AES-128 CBC Encryption**: Military-grade security
- **PKCS7 Padding**: Industry-standard block cipher padding
- **Message Counter**: Replay attack prevention
- **XOR Fallback**: Lightweight encryption alternative

</td>
</tr>
</table>

### ✨ Additional Capabilities
- 📱 **Serial Interface**: Easy debugging and message input via UART
- 🔌 **Multi-MCU Support**: ESP8266, ESP32, Arduino Nano/Uno compatible
- ⚡ **OTA Ready**: Over-The-Air update capability
- 🎯 **Plug & Play**: Simple serial communication for other MCUs

---

## 🛠️ Hardware Requirements

### Core Components
| Component | Specification | Quantity |
|-----------|--------------|----------|
| ESP8266 | NodeMCU / Wemos D1 Mini | 2+ |
| LoRa SX1278 | 433MHz Module | 2+ |
| Jumper Wires | Male-to-Female | 8 per unit |
| Power Supply | 3.3V (min 500mA) | 1 per unit |

### Optional Components
- Breadboard for prototyping
- External antenna for extended range
- Level shifter (for 5V Arduino boards)

---

## 🔌 Wiring Diagram

### ESP8266 Configuration

| ESP8266 GPIO | SX1278 Pin | Description |
|--------------|------------|-------------|
| GPIO15 | NSS/CS | Chip Select |
| GPIO16 | RST | Reset |
| GPIO5 | DIO0 | Interrupt |
| GPIO14 | SCK | SPI Clock |
| GPIO13 | MOSI | SPI Data Out |
| GPIO12 | MISO | SPI Data In |
| 3.3V | VCC | Power Supply |
| GND | GND | Ground |

### Alternative MCU Configurations

<details>
<summary><b>ESP32 Configuration</b></summary>

#### Default Configuration
| ESP32 GPIO | SX1278 Pin | Description |
|------------|------------|-------------|
| GPIO5 | NSS/CS | Chip Select |
| GPIO14 | RST | Reset |
| GPIO2 | DIO0 | Interrupt |
| GPIO18 | SCK | SPI Clock |
| GPIO23 | MOSI | SPI Data Out |
| GPIO19 | MISO | SPI Data In |
| 3.3V | VCC | Power |
| GND | GND | Ground |

#### Alternative Configuration
| ESP32 GPIO | SX1278 Pin |
|------------|------------|
| GPIO15 | NSS/CS |
| GPIO4 | RST |
| GPIO26 | DIO0 |

</details>

<details>
<summary><b>Arduino Nano/Uno Configuration</b></summary>

| Arduino Pin | SX1278 Pin | Description |
|-------------|------------|-------------|
| D10 | NSS/CS | Chip Select |
| D9 | RST | Reset |
| D2 | DIO0 | Interrupt (INT0) |
| D13 | SCK | SPI Clock |
| D11 | MOSI | SPI Data Out |
| D12 | MISO | SPI Data In |
| 3.3V | VCC | Power |
| GND | GND | Ground |

⚠️ **Important**: Arduino boards are 5V devices. Use the 3.3V output pin for LoRa power supply!

</details>

---

## 📥 Installation

### 1️⃣ Arduino IDE Setup

```bash
# Add ESP8266 Board Package
# File → Preferences → Additional Board Manager URLs:
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

### 2️⃣ Install Required Libraries

Navigate to **Sketch → Include Library → Manage Libraries** and install:

- **LoRa** by Sandeep Mistry
- **AESLib** by DavyLandman

### 3️⃣ Clone Repository

```bash
git clone https://github.com/roboticist-blip/LoRa8266T.git
cd LoRa8266T
```

### 4️⃣ Upload Firmware

1. Open `LoRa8266T.ino` in Arduino IDE
2. Select your board: **Tools → Board → ESP8266 Boards → NodeMCU 1.0**
3. Select COM port: **Tools → Port**
4. Click **Upload** ⬆️

---

## 🚀 Usage

### Quick Start

1. **Power up** both ESP8266 + LoRa modules
2. **Open Serial Monitor** at 9600 baud
3. **Type message** and press Enter
4. **Watch magic happen** ✨

### Example Output

```
ESP8266 LoRa Interface with AES Encryption Starting...
LoRa Ready with AES Encryption!

TX: 0:Hello World (Encrypted)
RX: 1:Hello back! (RSSI: -45)
TX: 2:Testing secure comms
RX: 3:Roger that! (RSSI: -52)
```

### Message Format
- **TX**: Transmitted message with counter
- **RX**: Received message with RSSI (signal strength)
- **RSSI Values**: -30 to -60 (Excellent), -60 to -90 (Good), < -90 (Poor)

---

## ⚙️ Configuration

### LoRa Parameters

```cpp
#define LORA_FREQUENCY 433E6          // 433 MHz (Europe/Asia)
#define LORA_SYNC_WORD 0x12           // Network ID
#define LORA_SPREADING_FACTOR 7       // 7-12 (higher = longer range)
#define LORA_BANDWIDTH 125E3          // 125 kHz
#define LORA_TX_POWER 17              // dBm (2-20)
```

### Regional Frequency Bands

| Region | Frequency | Legal Status |
|--------|-----------|--------------|
| Europe/Asia | 433 MHz | ISM Band ✅ |
| North America | 915 MHz | ISM Band ✅ |
| Europe | 868 MHz | ISM Band ✅ |

### AES Encryption Key

```cpp
uint8_t aes_key[AES_KEY_SIZE] = {
    0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
    0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
```

⚠️ **Security Warning**: Change the default key for production use!

### Performance Tuning

**For Maximum Range:**
```cpp
#define LORA_SPREADING_FACTOR 12      // Slower but longer range
#define LORA_TX_POWER 20              // Maximum power
```

**For Maximum Speed:**
```cpp
#define LORA_SPREADING_FACTOR 7       // Faster transmission
#define LORA_BANDWIDTH 250E3          // Wider bandwidth
```

---

## 🔒 Security Features

### Encryption Pipeline

```
Plaintext → AES-128 CBC → PKCS7 Padding → LoRa Transmission
```

### Security Components

1. **AES-128 CBC**: Symmetric encryption with 128-bit key
2. **Initialization Vector (IV)**: Randomized per message
3. **Message Counter**: Monotonically increasing sequence
4. **PKCS7 Padding**: Ensures block alignment

### Attack Mitigation

✅ **Replay Attack**: Prevented by message counter  
✅ **Man-in-the-Middle**: Protected by AES encryption  
✅ **Eavesdropping**: Encrypted transmission  
⚠️ **Key Distribution**: Manual key sharing required

---

## 🐛 Troubleshooting

### Common Issues

| Problem | Solution |
|---------|----------|
| ❌ LoRa init failed | • Check wiring connections<br>• Verify 3.3V power supply<br>• Test with multimeter |
| 📡 No messages received | • Match frequency on both devices<br>• Verify sync word (0x12)<br>• Check antenna connection |
| 🔓 Decryption failed | • Ensure identical AES keys<br>• Verify both devices running same code |
| 📉 Poor range | • Adjust antenna position<br>• Increase TX power<br>• Use higher spreading factor |
| 💡 ESP8266 won't boot | • Check GPIO15 not floating<br>• Verify GPIO0/GPIO2 states<br>• Test power supply |

### Debug Mode

Enable detailed logging:
```cpp
#define DEBUG_MODE 1
```

---

## 📊 Technical Specifications

| Parameter | Value |
|-----------|-------|
| **Range** | Up to 2km (line-of-sight) |
| **Data Rate** | ~5.5 kbps @ SF7 |
| **Frequency** | 433 MHz (configurable) |
| **Encryption** | AES-128 CBC |
| **Power Consumption** | ~100mA TX, ~15mA RX |
| **Operating Voltage** | 3.3V |
| **Interface** | UART (9600 baud) |

---

## 📚 Documentation

### File Structure
```
LoRa8266T/
├── LoRa8266T.ino          # Main firmware
├── docs/
│   ├── wiring_diagram.png # Connection guide
│   └── LICENSE            # MIT License
└── README.md              # This file
```

### Pin Mapping Reference

<details>
<summary><b>View Complete Pin Configurations</b></summary>

#### ESP8266 Code Example
```cpp
#define SS_PIN 15      // GPIO15
#define RST_PIN 16     // GPIO16
#define DIO0_PIN 5     // GPIO5
```

#### Alternative ESP8266 Pins
```cpp
#define SS_PIN 4       // GPIO4 (D2)
#define RST_PIN 0      // GPIO0 (D3)
#define DIO0_PIN 2     // GPIO2 (D4)
```

</details>

---

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. 🍴 **Fork** the repository
2. 🌿 **Create** your feature branch: `git checkout -b feature/amazing-feature`
3. 💾 **Commit** your changes: `git commit -m 'Add amazing feature'`
4. 📤 **Push** to the branch: `git push origin feature/amazing-feature`
5. 🎉 **Open** a Pull Request

### Development Guidelines

- Follow existing code style
- Test on real hardware before submitting
- Update documentation for new features
- Add comments for complex logic

---

## 📜 License

This project is licensed under the **MIT License** - see the [LICENSE](docs/LICENSE) file for details.

### What This Means
✅ Commercial use  
✅ Modification  
✅ Distribution  
✅ Private use  

---

## 🙏 Acknowledgments

- **Sandeep Mistry** - [LoRa Library](https://github.com/sandeepmistry/arduino-LoRa)
- **DavyLandman** - [AESLib](https://github.com/DavyLandman/AESLib)
- **ESP8266 Community** - Hardware support and documentation

---

## ⚖️ Legal Disclaimer

⚠️ **Important Notices:**

- This project is for **educational and experimental purposes**
- Ensure compliance with **local radio regulations** when using LoRa frequencies
- The default AES key is for **demonstration only**
- Always use your **own secure key** in production
- Respect ISM band regulations in your region
- Maximum transmission power may be legally restricted

---

## 🌐 Connect & Support

<div align="center">

### ⭐ Star this repository if you find it useful!

[![GitHub Stars](https://img.shields.io/github/stars/roboticist-blip/LoRa8266T?style=social)](https://github.com/roboticist-blip/LoRa8266T/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/roboticist-blip/LoRa8266T?style=social)](https://github.com/roboticist-blip/LoRa8266T/network/members)
[![GitHub Issues](https://img.shields.io/github/issues/roboticist-blip/LoRa8266T)](https://github.com/roboticist-blip/LoRa8266T/issues)

**Questions?** Open an [issue](https://github.com/roboticist-blip/LoRa8266T/issues)  
**Ideas?** Start a [discussion](https://github.com/roboticist-blip/LoRa8266T/discussions)

</div>

---

<div align="center">

**Made with ❤️ for the IoT Community**

*Built with ESP8266 • Secured with AES-128 • Powered by LoRa*

</div>
