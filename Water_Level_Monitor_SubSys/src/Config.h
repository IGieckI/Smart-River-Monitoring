#ifndef __CONFIG_H__
#define __CONFIG_H__

// Digital Pins
#define RED_LED_PIN 4
#define GREEN_LED_PIN 5
#define DISTANCE_SENSOR_ECHO_PIN 6
#define DISTANCE_SENSOR_TRIG_PIN 7

// Distance sensor settings
#define MAX_DISTANCE_TIME 1000000l // microsecond time limit to wait for a distance sensor range
#define MAX_DISTANCE 26
#define MAX_ERROR_DISTANCE 1000 // If distance > MAX_ERROR_DISTANCE, then the distance sensor is ignored (probably an error)

// Other Constants
#define BAUD_RATE 115200

#define WIFI_SSID "SmartRiverMonitoring"
#define WIFI_PASSWORD "password123"

#define MQTT_SERVER "broker.mqtt-dashboard.com"
#define MQTT_PORT 1883
#define MQTT_INITIALIZATION_TOPIC "water_level_iot_24h/protocol"

#endif