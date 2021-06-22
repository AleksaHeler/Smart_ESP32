///// TODO: /////////////////
//   rgb status light
//   internet time getter
//   internet weather forecast
//   OTA updates (can be enabled/disabled)
//   safe MQTT (with users and passwords)

// WIRING:
//   > BMP280:
//       SCL -> 22
//       SDA -> 21
//   > PIR -> 18
//   > Light -> 34


//// Defines ////////////////////////////////////////////////////////////////////
#define ENABLE_WIFI
//#define ENABLE_OTA              // Over-the-air code update
#define ENABLE_BMP280           // Temperature and pressure sensor
#define ENABLE_PIR              // Passive infrared sensor
#define ENABLE_LIGHT            // Resistive light intensity sensor
#define ENABLE_DEEP_SLEEP       // Deep sleep to save battery

#define DEVICE_NAME "ESP32"
#define PUBLISH_INTERVAL 10*60  // seconds (10 minutes)
#define US_TO_S 1000000         // For converting microseconds to seconds


//// Local header files /////////////////////////////////////////////////////////
#include "system.h"         // Hardware specific things (pins...)
#include "ESP32_BMP280.h"   // BMP280 pressure/temperature sensor
#include "ESP32_PIR.h"      // Passive infrared sensor (motion)
#include "ESP32_Light.h"    // Resistive light intensity sensor
#include "ESP32_WiFi.h"     // WiFi and MQTT 

#ifdef ENABLE_DEEP_SLEEP
RTC_DATA_ATTR int bootCount = 0;
#endif

#ifdef ENABLE_DEEP_SLEEP
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
#endif

//// setup() and loop() functions /////////////////////////////////////////////////////////
void setup() {
    Serial.begin(115200);
    Serial.println("Starting setup...");

    #ifdef ENABLE_BMP280
    initialize_BMP280();
    #endif

    #ifdef ENABLE_WIFI
    initialize_wifi();
    intialize_mqtt();
    #endif

    initialize_pins();
    Serial.println("Done setting up!");

    #ifdef ENABLE_DEEP_SLEEP
        bootCount++;
        Serial.println("Boot count: " + String(bootCount));
        print_wakeup_reason();
        esp_sleep_enable_timer_wakeup(PUBLISH_INTERVAL * US_TO_S);
        Serial.println("Sending data...");
  
        #ifdef ENABLE_WIFI
        handle_wifi();
        mqtt_publish();
        Serial.println("Done sending data");
        #endif
        
        Serial.println("Going to sleep now");
        delay(500);
        Serial.flush();
        delay(500);
        esp_deep_sleep_start();
    #endif
}

void loop() {
    #ifndef ENABLE_DEEP_SLEEP
        time_now = millis();
    
        // Non-blocking delay
        if (time_now - time_previous >= PUBLISH_INTERVAL*1000) {
            Serial.println("Sending data...");
            time_previous = time_now;
    
            #ifdef ENABLE_WIFI
            handle_wifi();
            mqtt_publish();
            Serial.println("Done sending data");
            #endif
        }
      
  
        #if defined ENABLE_WIFI && defined ENABLE_OTA
        ArduinoOTA.handle();
        //Serial.println("Handle OTA");
        #endif
    #endif
}
