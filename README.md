# WSPR_beacon_ESP32 (GPS)

It allows you to create a low-cost multiband WSPR TX (beacon) with an SI5351 board (8 kHz to 160 MHz with an output power of 8 dBm - 6 mW) and 
an ESP32 with GPS like the Lilygo ESP32 LoRa GPS 433MHz (Lilygo T-Beam).

The program allows you to follow the operation of the WSPR TX using the OLED screen (if your board has one, it is very useful) 
and through the serial output if it is connected to a computer.

The program allows the implementation of the international WSPR beacon project standard (https://github.com/HB9VQQ/WSPRBeacon)

These programs are not necessary for the operation of WSPR beacon, only to use the OLED screen if you have it and want it:
- Adafruit_SSD1306.cpp
- Adafruit_SSD1306.h
- Fixed8x16.h

It's highly recommended to have:
- An RF receiver (e.g., SDR) to verify that the WSPR TX is transmitting and on what frequency
- A WSPR receiver to monitor WSPR or SPOTS transmissions

REMEMBER:  that to operate a WSPR beacon you need to have an amateur radio license. If you do not have one, it is very practical to have one and it is easier to obtain than a driver's license.

More in spanish: https://ea5jtt.blogspot.com/2025/06/wspr-tx-arduino-si5351.html
