# Useful links
- [Useful links](#useful-links)
  - [Installation](#installation)
    - [ble sniffer](#ble-sniffer)
    - [platformIO configuration](#platformio-configuration)
  - [Sniffing](#sniffing)
  - [Software](#software)
    - [esp32](#esp32)
    - [](#)
  - [Hardware](#hardware)

## Installation
### ble sniffer
* [nordic official nRF sniffer docs][nordic installation guide]  
* [adafruit installation guide (similar to nordic's docs but worth mentioning)][adafruit]  
* [adafruit forum with a solution to not seeing any packets][adafruit forum #1]
* [adafruit forum with a solution to seeing only advertising packets][adafruit forum #2]

### platformIO configuration
* [Using c++ 17][platform io c++ 17 forum]
## Sniffing
* [nordic's simple guide on sniffing][nordic sniffing guide]
* [brilliant writeup on ble security that gives an overview on how ble communication works][Understanding bluetooth security]
* [more detailed look on ble pairing][BLE Pairing and Bonding]

## Software
### esp32
* [Esp32 queue][esp32 queue]
* [Esp32 multi centrals (clients)][multi centrals]
* [Esp32 messagebuffer][messagebuffer]
* [Esp32 message buffer mini example][messagebuffer example]
* [Esp32 Eliptic curve Diffie Hellman example][esp32 ECDHE]
* [Esp32 AES example][Esp32 AES]
* [ESP32 Hmac example][Esp32 hmac]
* [Esp32 BLE guide][Esp32 ble]
### 
* [forum discussion on security][security]
## Hardware
* [powering leds][adafruit power guide]

[adafruit]: https://learn.adafruit.com/introducing-the-adafruit-bluefruit-le-sniffer/using-with-sniffer-v2-and-python3  
[nordic installation guide]: https://infocenter.nordicsemi.com/index.jsp?topic=%2Fug_sniffer_ble%2FUG%2Fsniffer_ble%2Fintro.html  
[adafruit forum #1]: https://forums.adafruit.com/viewtopic.php?p=949843#p949843  
[adafruit forum #2]: https://forums.adafruit.com/viewtopic.php?p=953020#p953020
[nordic sniffing guide]: https://infocenter.nordicsemi.com/pdf/nRF_Sniffer_UG_v2.2.pdf
[Understanding bluetooth security]: https://duo.com/decipher/understanding-bluetooth-security
[BLE Pairing and Bonding]: https://technotes.kynetics.com/2018/BLE_Pairing_and_bonding/
[esp32 queue]: https://www.dfrobot.com/blog-912.html
[platform io c++ 17 forum]: https://community.platformio.org/t/esp32-c-17-toolchain-missing-std-optional/25850/6
[adafruit power guide]: https://learn.adafruit.com/adafruit-neopixel-uberguide/powering-neopixels
[multi centrals]: https://github.com/nkolban/esp32-snippets/issues/137
[messagebuffer]: https://www.freertos.org/RTOS-message-buffer-example.html
[messagebuffer example]: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos_idf.html#message-buffer-api
[esp32 ECDHE]: https://primalcortex.wordpress.com/2020/05/05/esp8266nodejs-ecdh/ 
[esp32 AES]: https://www.dfrobot.com/blog-1001.html
[esp32 hmac]: https://techtutorialsx.com/2018/01/25/esp32-arduino-applying-the-hmac-sha-256-mechanism/
[security]: https://security.stackexchange.com/questions/166063/tls-rsa-vs-tls-ecdhe-rsa-vs-static-dh
[esp32 ble]: https://github.com/nkolban/esp32-snippets/blob/master/Documentation/BLE%20C%2B%2B%20Guide.pdf