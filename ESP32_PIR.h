
#ifdef ENABLE_PIR

int pir_active = 0;

void read_pir(){
    pir_active = digitalRead(pirSensorPin);
}

void pir_format_mqtt_message(char* message, char* topic){
    char value[64];                                 // Value used for string conversion

    read_pir();                                     // Get sensor values

    memset(message,0,512);                          // Clear buffers
    memset(topic,0,64);
    strcpy(topic, DEVICE_NAME);                     // Set topic to "ESP32/BMP280"
    strcat(topic, "/PIR");

    strcpy(message, "PIR=");                        // Set variable name in buffer
    value[0] = pir_active == 0 ? '0' : '1';         // Set variable value in buffer
    value[1] = '\0';
    strcat(message, value);
}


#endif // ENABLE_PIR