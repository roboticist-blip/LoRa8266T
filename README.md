# ESP8266 LoRa Encrypted Communication

A secure wireless communication system using ESP8266 microcontrollers and LoRa SX1278 modules with AES encryption.

## Features

- **Long Range Communication**: LoRa 433MHz for extended range
- **AES-128 Encryption**: Secure message transmission with PKCS7 padding
- **Message Counter**: Built-in message sequencing
- **RSSI Monitoring**: Signal strength indication
- **Serial Interface**: Easy debugging and message input
- **Fallback XOR Encryption**: Lightweight alternative encryption method

## Hardware Requirements

### Components
- ESP8266 development board (NodeMCU, Wemos D1 Mini, etc.)
- LoRa SX1278 module (433MHz)
- Jumper wires
- Breadboard (optional)

### Wiring Diagram

| ESP8266 GPIO | SX1278 Pin | Description |
|--------------|------------|-------------|
| GPIO15       | NSS/CS     | Chip Select |
| GPIO16       | RST        | Reset       |
| GPIO5        | DIO0       | Interrupt   |
| 3.3V         | VCC        | Power       |
| GND          | GND        | Ground      |
| GPIO14       | SCK        | SPI Clock   |
| GPIO13       | MOSI       | SPI Data Out|
| GPIO12       | MISO       | SPI Data In |

## Software Requirements

### Arduino IDE Setup
1. Install ESP8266 board package in Arduino IDE
2. Install required libraries:
   - `LoRa` by Sandeep Mistry
   - `AESLib` by DavyLandman

### Library Installation
```
Arduino IDE → Sketch → Include Library → Manage Libraries
Search and install:
- LoRa
- AESLib
```

## Configuration

### LoRa Settings
```cpp
#define LORA_FREQUENCY    433E6  // 433 MHz
#define LORA_SYNC_WORD    0x12
#define LORA_SPREADING_FACTOR 7
#define LORA_BANDWIDTH    125E3
#define LORA_TX_POWER     17
```

### Security Configuration
⚠️ **IMPORTANT**: Change the default AES key before deployment!

```cpp
uint8_t aes_key[AES_KEY_SIZE] = {
  0x2B, 0x7E, 0x15, 0x16, 0x28, 0xAE, 0xD2, 0xA6,
  0xAB, 0xF7, 0x15, 0x88, 0x09, 0xCF, 0x4F, 0x3C
};
```

## Usage

1. **Upload the code** to two or more ESP8266 devices
2. **Open Serial Monitor** at 9600 baud rate
3. **Type messages** in the serial monitor and press Enter
4. **Messages are automatically encrypted** and transmitted via LoRa
5. **Received messages are decrypted** and displayed with RSSI values

### Example Output
```
ESP8266 LoRa Interface with AES Encryption Starting...
LoRa Ready with AES Encryption!
TX: 0:Hello World (Encrypted)
RX: 1:Hello back! (RSSI: -45)
```

## Security Features

- **AES-128 CBC Encryption**: Industry-standard encryption
- **PKCS7 Padding**: Proper block cipher padding
- **Message Counter**: Prevents replay attacks
- **IV Generation**: Initialization vector for encryption
- **XOR Fallback**: Lightweight encryption option

## Customization

### Changing Frequency
Modify `LORA_FREQUENCY` to match your regional ISM band:
- 433MHz (Europe, Asia)
- 915MHz (North America)
- 868MHz (Europe)

### Adjusting Range vs Speed
- **Longer Range**: Increase spreading factor (7-12)
- **Faster Speed**: Decrease spreading factor, increase bandwidth

## Troubleshooting

### Common Issues
1. **LoRa init failed**: Check wiring connections
2. **No messages received**: Verify frequency and sync word match
3. **Decryption failed**: Ensure both devices use the same AES key
4. **Poor range**: Check antenna connection and TX power settings

### Debug Tips
- Monitor serial output for TX/RX confirmations
- Check RSSI values for signal quality
- Verify LoRa module power supply (3.3V, sufficient current)

## Performance

- **Range**: Up to 2km line-of-sight (depends on environment)
- **Data Rate**: ~5.5kbps at SF7
- **Power Consumption**: ~100mA during transmission

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

### Development Setup
1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- LoRa library by Sandeep Mistry
- AESLib by DavyLandman
- ESP8266 Community

## Disclaimer

This project is for educational and experimental purposes. Ensure compliance with local radio regulations when using LoRa frequencies. The default AES key is for demonstration only - always use your own secure key in production.

---

**⭐ Star this repository if you find it useful!**
