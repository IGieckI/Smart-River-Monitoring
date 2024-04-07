#include "WaterLevelMonitor.h"

WaterLevelMonitor::WaterLevelMonitor(uint8_t redLedPin, uint8_t greenLedPin, uint8_t echoPin, uint8_t trigPin, int maxDistanceTime) {
    greenLed = new Led(greenLedPin);
    redLed = new Led(redLedPin);
    distanceSensor = new Sonar(echoPin, trigPin, maxDistanceTime);
}

void WaterLevelMonitor::switchGreenLed(bool state) {
    greenLed->switchLight(state);
}

void WaterLevelMonitor::switchRedLed(bool state) {
    redLed->switchLight(state);
}

unsigned short WaterLevelMonitor::getWaterLevel(int minDistance) {
    unsigned short distance = distanceSensor->getDistance();
    
    if (distance < minDistance) {
        return 0;
    } else {
        return distance;
    }
}