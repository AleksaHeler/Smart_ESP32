# Smart ESP32

An IoT oriented application of ESP32 microcontroller. This is a modular script (work in progress) used for interfacing sensors with MQTT broker and database on RaspberryPi.

## Prerequisites

Arduino IDE. Also add a file called ``` credentials.h ``` and paste this code (and change data):
``` c
// Replace the next variables with your WiFi SSID/Password combination
const char* ssid = "your_SSID";
const char* password = "your_password";

// Over the air updates
const char* otaHostname = "ESP32_Hostname";
const char* otaPassword = "OTA_password";
```