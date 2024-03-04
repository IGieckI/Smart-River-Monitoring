#ifndef __CONFIG_H__
#define __CONFIG_H__

// Digital Pins
#define LED1_PIN 3
#define LED2_PIN 4
#define DISTANCE_SENSOR_TRIG_PIN 10
#define DISTANCE_SENSOR_ECHO_PIN 11

// Other Constants
#define BAUD_RATE 115200
#define UPDATE_FREQUENCY 500 // millisecond update frequency time
#define MAX_DISTANCE_TIME 1000000l // microsecond time limit to wait for a distance sensor range
#define MIN_DISTANCE 6
#define MAX_DISTANCE 26
#define MAX_ERROR_DISTANCE 30 // If distance > MAX_ERROR_DISTANCE, then the distance sensor is not reading correctly


#endif