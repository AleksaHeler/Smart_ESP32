/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

#include "credentials.h"

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.0.200";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

//uncomment the following lines if you're using SPI
/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

Adafruit_BMP280 bmp; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
float temperature = 0;
float pressure = 0;
float altitude = 0;

const int analogTempPin1 = 25;
const int analogTempPin = 5;
const int lightSensorPin = 13;
const int pirSensorPin = 18; 

void setup() {
  Serial.begin(115200);
  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  //status = bme.begin();  
  
  if (!bmp.begin(0x76, BMP280_CHIPID)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                      "try a different address!"));
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(analogTempPin, INPUT);
  pinMode(analogTempPin1, INPUT);
  pinMode(lightSensorPin, INPUT);
  pinMode(pirSensorPin, INPUT);
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
    }
    else if(messageTemp == "off"){
      Serial.println("off");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  unsigned long sum = 0;
  for(int i = 0; i < 10; i++){
    sum += analogRead(analogTempPin);
  }
  float analogTemp = 3.3*(sum / 10.0)/4096.0 / 0.01 / 9.6;
  float lightSensor = (4096-analogRead(lightSensorPin))/1000.0;  // Inverted voltage
  int pirSensor = digitalRead(pirSensorPin);
  /*Serial.print("************\nAnalog temp: ");
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
  Serial.println("m");*/
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
    
    /*message.concat(";ATEMP=");
    char analogTempString[8];
    dtostrf(analogTemp, 1, 2, analogTempString);
    message.concat(analogTempString);*/
    
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
  }
}
