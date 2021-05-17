/* 
*  WiFi and MQTT specific code 
*  Functions: initialize wifi, initialize mqtt, handle wifi, mqtt publish, mqtt callback, wifi reconnect
*/

#ifdef ENABLE_WIFI

#include <WiFi.h>
#include <PubSubClient.h>

#include "credentials.h"

void initialize_wifi();
void intialize_mqtt();
void mqtt_publish();
void handle_wifi();
void callback(char*, byte*, unsigned int);
void reconnect();

// MQTT Broker IP address
const char* mqtt_server = "192.168.0.200";

WiFiClient espClient;
PubSubClient client(espClient);


// Connect to given WiFi network
void initialize_wifi() {
    delay(10);
    Serial.printf("\nConnecting to %s", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.printf("\nWiFi connected! [%s]\n", WiFi.localIP());
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
    Serial.printf("MQTT publish: [%s] {%s}\n", topic, message);     // Debug
    client.publish(topic, message);                                 // Publish
    #endif

    #ifdef ENABLE_PIR
    pir_format_mqtt_message(message, topic);
    Serial.printf("MQTT publish: [%s] {%s}\n", topic, message);
    client.publish(topic, message);
    #endif
}

void handle_wifi(){
    if (!client.connected())
        reconnect();
    client.loop();
}

void callback(char* topic, byte* message, unsigned int length) {
    Serial.printf("MQTT callback: [%s] {%s}\n", topic, message);
}

void reconnect() {
    while (!client.connected()) {                               // MQTT callback
        Serial.print("Attempting MQTT connection...");
        if (client.connect("ESP8266Client")) {                  // Attempt to connect
            Serial.println("connected");
            client.subscribe("ESP32/output");                   // Subscribe
        } else {
            Serial.printf("failed, rc=%d. Try again in 5 seconds\n", client.state());
            delay(5000);
        }
    }
}

#endif // ENABLE_WIFI
