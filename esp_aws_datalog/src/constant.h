#ifndef CONSTANT_H_
#define CONSTANT_H_
#include <Arduino.h>



// WiFi credentials
const char WIFI_SSID[] = "Pi_01";
const char WIFI_PASSWORD[] = "11111111";

// Device name from AWS
const char THINGNAME[] = "ESP8266";

// MQTT broker host address from AWS
//const char MQTT_HOST[] = "xxxxxxxx-ats.iot.us-east-1.amazonaws.com";

// MQTT topics
const char AWS_IOT_PUBLISH_TOPIC[] = "esp8266/pub";
const char AWS_IOT_SUBSCRIBE_TOPIC[] = "esp8266/sub";

// Publishing interval
const long interval = 5000;

// Timezone offset from UTC
const int8_t TIME_ZONE = 7;


typedef enum {
    ERROR,
    SUCCESS
}status_t;

#endif /*CONSTANT_H_*/