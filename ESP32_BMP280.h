/* 
*  Interfacing BMP280 temperature and barometric pressure sensor using I2C 
*  Functions: initialize BMP280, read values
*/

#ifdef ENABLE_BMP280

#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_Sensor.h>

void initialize_BMP280();
void read_BMP280();

Adafruit_BMP280 bmp;
const unsigned int bmp_address = 0x76;  // I2C address

float bmp280_temperature = 0;   // Celsius
float bmp280_pressure = 0;      // mBar
float bmp280_altitude = 0;      // meters

// Initializes temp/pressure sensor, and if that fails, loops continuously
void initialize_BMP280(){
    if (!bmp.begin(bmp_address, BMP280_CHIPID)) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
        while(1) delay(10);
    }

    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     // Operating Mode.
                    Adafruit_BMP280::SAMPLING_X2,     // Temp. oversampling
                    Adafruit_BMP280::SAMPLING_X16,    // Pressure oversampling
                    Adafruit_BMP280::FILTER_X16,      // Filtering
                    Adafruit_BMP280::STANDBY_MS_500); // Standby time
}

void read_BMP280(){
    bmp280_temperature = bmp.readTemperature();
    bmp280_pressure = bmp.readPressure()/100.0; // Convert from pascals to mBar
    bmp280_altitude = bmp.readAltitude(1013.25);
}

void bmp280_format_mqtt_message(char* message, char* topic){
    char value[64];                                 // Value used for float->string conversion

    read_BMP280();                                  // Get sensor values

    memset(message, 0, 512);                        // Clear buffer
    memset(topic, 0, 64);
    strcpy(topic, DEVICE_NAME);                     // Set topic to "ESP32/BMP280"
    strcat(topic, "/BMP280");

    strcpy(message, "TEMP=");                       // Set variable name in buffer
    ftoa(bmp280_temperature, value, 2);             // Convert float to string
    strcat(message, value);                         // Add value string to buffer

    strcat(message, ";PRES=");                      // Pressure
    ftoa(bmp280_pressure, value, 1);
    strcat(message, value);

    strcat(message, ";ALT=");                       // Altitude
    ftoa(bmp280_altitude, value, 2);
    strcat(message, value);
}

#endif // ENABLE_BMP280