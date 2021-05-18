/* 
*  WiFi and MQTT specific code 
*  Functions: initialize wifi, initialize mqtt, handle wifi, mqtt publish, mqtt callback, wifi reconnect
*/
#ifdef ENABLE_WIFI


#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#include "credentials.h"


//// Function declarations /////////////////////////////////////////////////////////
void initialize_wifi();
void intialize_mqtt();
void mqtt_publish();
void handle_wifi();
void callback(char*, byte*, unsigned int);
void reconnect();


//// Variables /////////////////////////////////////////////////////////////////////
const char* mqtt_server = "192.168.0.200";      // MQTT Broker IP address
WiFiClient espClient;
PubSubClient client(espClient);


//// Functions /////////////////////////////////////////////////////////////////////
// Connect to given WiFi network
void initialize_wifi() {
    delay(10);
    Serial.printf("\nConnecting to %s", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nWiFi connected!");

    #ifdef ENABLE_OTA
    ArduinoOTA.setHostname(otaHostname);
    ArduinoOTA.setPassword(otaPassword);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_FS
            type = "filesystem";
        }
        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
    #endif
}


void intialize_mqtt(){
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}


void mqtt_publish(){
    char message[512];                              // Buffer used to publish 
    char topic[64];                                 // Topic for MQTT

    #ifdef ENABLE_BMP280
    bmp280_format_mqtt_message(message, topic);                     // Formating is done inside sensor header file
    //Serial.printf("MQTT publish: [%s] {%s}\n", topic, message);   // Debug
    client.publish(topic, message);                                 // Publish
    #endif

    #ifdef ENABLE_PIR
    pir_format_mqtt_message(message, topic);
    //Serial.printf("MQTT publish: [%s] {%s}\n", topic, message);
    client.publish(topic, message);
    #endif

    #ifdef ENABLE_LIGHT
    light_format_mqtt_message(message, topic);
    //Serial.printf("MQTT publish: [%s] {%s}\n", topic, message);
    client.publish(topic, message);
    #endif
}


void handle_wifi(){
    if (!client.connected())
        reconnect();
    client.loop();
}


void callback(char* topic, byte* message, unsigned int length) {
    message[length] = '\0';
    Serial.printf("MQTT [%s] {%s}\n", topic, message);
}


void reconnect() {
    while (!client.connected()) {                               // MQTT callback
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client")) {                  // Attempt to connect
            Serial.println("connected");
            client.subscribe("#");                              // Subscribe to all topics
        } else {
            Serial.printf("failed, rc=%d. Try again in 5 seconds\n", client.state());
            delay(5000);
        }
    }
}


#endif // ENABLE_WIFI
