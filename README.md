# Smart ESP32

An IoT oriented application of ESP32 microcontroller. This is a modular script (work in progress) used for interfacing sensors with MQTT broker and database on RaspberryPi.

## Prerequisites

Arduino IDE or Visual Studio Code (what I'm using). Also add a file called ``` credentials.h ``` and paste this code (and change data):
``` c
// Replace the next variables with your WiFi SSID/Password combination
const char* ssid = "your_SSID";
const char* password = "your_password";

// Over the air updates
const char* otaHostname = "ESP32_Hostname";
const char* otaPassword = "OTA_password";
```
To have a build folder (faster builds when little has changed), you can add this line to ```./vscode/arduino.json```
``` js
"output": "build"
```
If you would also like to use over-the-air (OTA) sketch updates, you need to add ``` UploadTool=esptool ``` to the line with ```"configuration": ... ``` inside ```./vscode/arduino.json```. Then the file ``` arduino.json ``` should look something like this:
``` js
{
    "configuration": "UploadTool=esptool,PSRAM=disabled,PartitionScheme=default,CPUFreq=240,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none",
    "board": "esp32:esp32:esp32",
    "port": "COM3",
    "sketch": "Smart_ESP32.ino",
    "output": "build"
}
```
