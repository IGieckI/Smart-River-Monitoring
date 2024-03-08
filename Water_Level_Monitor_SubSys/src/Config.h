#ifndef __CONFIG_H__
#define __CONFIG_H__

// Digital Pins
#define RED_LED_PIN 4
#define GREEN_LED_PIN 5
#define DISTANCE_SENSOR_ECHO_PIN 6
#define DISTANCE_SENSOR_TRIG_PIN 7

// Distance sensor settings
#define MAX_DISTANCE_TIME 1000000l // microsecond time limit to wait for a distance sensor range
#define MIN_DISTANCE 6
#define MAX_DISTANCE 26
#define MAX_ERROR_DISTANCE 30 // If distance > MAX_ERROR_DISTANCE, then the distance sensor is not reading correctly

// Other Constants
#define BAUD_RATE 115200
#define UPDATE_FREQUENCY 1000 // millisecond update frequency time

#define WIFI_SSID "Focantenna"//"SmartRiverMonitoring"
#define WIFI_PASSWORD "zprv9600"//"password123"

#define MQTT_SERVER "192.168.175.88" //"192.168.1.252"
#define MQTT_PORT 8883
#define MQTT_TOPIC "water_level"

#endif