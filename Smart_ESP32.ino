///// TODO: /////////////////
//   rgb status light
//   internet time getter
//   internet weather forecast
//   OTA updates (can be enabled/disabled)
//   safe MQTT (with users and passwords)


//// Defines ////////////////////////////////////////////////////////////////////
#define ENABLE_WIFI
#define ENABLE_OTA          // Over-the-air code update
#define ENABLE_BMP280       // Temperature and pressure sensor
#define ENABLE_PIR          // Passive infrared sensor
#define ENABLE_LIGHT        // Resistive light intensity sensor

#define DEVICE_NAME "ESP32"
#define PUBLISH_INTERVAL 10 // seconds


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
    time_now = millis();

    // Non-blocking delay
    if (time_now - time_previous >= PUBLISH_INTERVAL*1000) {
        time_previous = time_now;

        #ifdef ENABLE_WIFI
        handle_wifi();
        mqtt_publish();
        #endif
    }
    
    #if defined ENABLE_WIFI && defined ENABLE_OTA
    ArduinoOTA.handle();
    #endif
}
