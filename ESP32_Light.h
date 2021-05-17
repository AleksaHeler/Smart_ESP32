/* 
*  Interfacing passive infrared (PIR) motion sensor using one digital read pin (active/unactive)
*/
#ifdef ENABLE_LIGHT


#define MEASURE_COUNT 10


//// Function declarations /////////////////////////////////////////////////////////
void read_light();
void light_format_mqtt_message();


//// Variables /////////////////////////////////////////////////////////////////////
float light_amount = 0;


//// Functions /////////////////////////////////////////////////////////////////////
void read_light(){
    // Read multiple times and take average
    unsigned long sum = 0;
    for(int i = 0; i < MEASURE_COUNT; i++){
        sum += analogRead(lightSensorPin);
    }
    light_amount = (4096.0 - (1.0 * sum / MEASURE_COUNT)))/4096.0;
}


void light_format_mqtt_message(char* message, char* topic){
    char value[64];                                 // Value used for string conversion

    read_light();                                   // Get sensor values

    memset(message,0,512);                          // Clear buffers
    memset(topic,0,64);
    strcpy(topic, DEVICE_NAME);                     // Set topic to "ESP32/BMP280"
    strcat(topic, "/light");

    strcpy(message, "LIGHT=");                      // Set variable name in buffer
    ftoa(light_amount, value, 2);                   // Convert float to string
    strcat(message, value);                         // Add value string to buffer
}


#endif // ENABLE_PIR