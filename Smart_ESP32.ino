///// TODO: /////////////////
//   rgb status light
//   internet time getter
//   internet weather forecast
//   OTA updates (can be enabled/disabled)
//   safe MQTT (with users and passwords)


//// Defines ////////////////////////////////////////////////////////////////////
#define ENABLE_WIFI
#define ENABLE_BMP280
#define ENABLE_PIR
#define ENABLE_LIGHT

#define DEVICE_NAME "ESP32"
#define PUBLISH_INTERVAL 60   // seconds


//// Local header files /////////////////////////////////////////////////////////
#include "system.h"         // Hardware specific things (pins...)
#include "ESP32_BMP280.h"   // BMP280 pressure/temperature sensor
#include "ESP32_PIR.h"      // Passive infrared sensor (motion)
#include "ESP32_Light.h"    // Resistive light intensity sensor
#include "ESP32_WiFi.h"     // WiFi and MQTT 


//// setup() and loop() functions /////////////////////////////////////////////////////////
void setup() {
    Serial.begin(115200);

    #ifdef ENABLE_BMP280
    initialize_BMP280();
    #endif

    #ifdef ENABLE_WIFI
    initialize_wifi();
    intialize_mqtt();
    #endif

    initialize_pins();
}

void loop() {
    delay(PUBLISH_INTERVAL * 1000); // TODO: make non blocking delay (if time passed > required...)

    #ifdef ENABLE_WIFI
    handle_wifi();
    mqtt_publish();
    #endif
}
