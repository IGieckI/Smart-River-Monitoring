#include "WaterLevelMonitor.h"

WaterLevelMonitor::WaterLevelMonitor() {
    greenLed = new Led(GREEN_LED_PIN);
    redLed = new Led(RED_LED_PIN);
    distanceSensor = new Sonar(DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);
}

Led *WaterLevelMonitor::getGreenLed() {
    return greenLed;
}

Led *WaterLevelMonitor::getRedLed() {
    return redLed;
}

int WaterLevelMonitor::getWaterLevel() {
    int distance = distanceSensor->getDistance();
    
    if (distance < MIN_DISTANCE) {
        return 0;
    } else {
        return distance;
    }
}