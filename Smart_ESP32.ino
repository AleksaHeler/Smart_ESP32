// TODO:
//   rgb status light
//   internet time getter
//   internet weather forecast
//   each sensor has its own mqtt topic to which the data is sent

#define ENABLE_WIFI
#define ENABLE_BMP280
#define ENABLE_PIR

#define DEVICE_NAME "ESP32"

/* Local header files */
#include "system.h"         // Hardware specific things (pins...)
#include "ESP32_BMP280.h"   // BMP280 pressure/temperature sensor
#include "ESP32_PIR.h"      // Passive infrared sensor (motion)
#include "ESP32_WiFi.h"     // WiFi and MQTT 

void setup() {
    Serial.begin(115200);

    #ifdef ENABLE_BMP280
    initialize_BMP280();
    #endif

    #ifdef ENABLE_WIFI
    initialize_wifi();
    intialize_mqtt();
    #endif

    //initialize_pins();
}


void loop() {
    delay(2000);

    #ifdef ENABLE_WIFI
    handle_wifi();
    mqtt_publish();
    #endif


    /*unsigned long sum = 0;
    for(int i = 0; i < 10; i++){
    sum += analogRead(analogTempPin);
    }
    float analogTemp = 3.3*(sum / 10.0)/4096.0 / 0.01 / 9.6;
    float lightSensor = (4096-analogRead(lightSensorPin))/1000.0;  // Inverted voltage
    int pirSensor = digitalRead(pirSensorPin);
    Serial.print("************\nAnalog temp: ");
    Serial.print(analogTemp);
    Serial.println("*C");
    Serial.print("Light sensor: ");
    Serial.println(lightSensor);
    Serial.print("PIR sensor: ");
    Serial.println(pirSensor);     // 0 - no activity : 1 - active
    Serial.print("BMP280 temp: ");
    Serial.print(bmp.readTemperature());
    Serial.println("*C");
    Serial.print("BMP280 pressure: ");
    Serial.print(bmp.readPressure()/100.0);
    Serial.println("mBar");
    Serial.print("BMP280 altitude: ");
    Serial.print(bmp.readAltitude(1013.25));
    Serial.println("m");
    delay(500);

    // message:  ESP32;ID=0043;TEMP=24.58;HUM=57;PRES=1013
    long now = millis();
    if (now - lastMsg > 1000*10) {
        String message = String(); // ESP32;ID=0043;TEMP=24.58;ALT=145;PRES=1013
        message.concat("ESP32;ID=0043;TEMP=");
        lastMsg = now;

        // Temperature in Celsius
        temperature = bmp.readTemperature();
        char tempString[8]; // Convert the value to a char array
        dtostrf(temperature, 1, 2, tempString);
        //Serial.print("Temperature: ");
        //Serial.println(tempString);
        //client.publish("esp32/temperature", tempString);
        message.concat(tempString);
        message.concat(";PRES=");


        // Convert the value to a char array
        pressure = bmp.readPressure()/100.0;
        char presString[8];
        dtostrf(pressure, 1, 2, presString);
        //Serial.print("Pressure: ");
        //Serial.println(presString);
        //client.publish("esp32/pressure", presString);
        message.concat(presString);
        message.concat(";ALT=");

        // Convert the value to a char array
        altitude = bmp.readAltitude(1013.25);
        char altString[8];
        dtostrf(altitude, 1, 2, altString);
        //Serial.print("Altitude: ");
        //Serial.println(altString);
        //client.publish("esp32/altitude", altString);
        message.concat(altString);

        message.concat(";ATEMP=");
        char analogTempString[8];
        dtostrf(analogTemp, 1, 2, analogTempString);
        message.concat(analogTempString);

        message.concat(";LIGHT=");
        char lightString[8];
        dtostrf(lightSensor, 1, 2, lightString);
        message.concat(lightString);

        message.concat(";PIR=");
        char pirString[2];
        dtostrf(pirSensor, 1, 2, pirString);
        message.concat(pirString);
        message.concat(";");

        Serial.print("Message: ");
        Serial.print(message);
        Serial.println("  Topic: bmp280");


        char messageCharArr[82];
        message.toCharArray(messageCharArr, 82);
        client.publish("bmp280", messageCharArr);
    }*/
}
