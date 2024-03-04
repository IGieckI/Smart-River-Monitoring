#include "WaterLevelMonitor.h"

WaterLevelMonitor::WaterLevelMonitor() {
    led1 = new Led(LED1_PIN);
    distanceSensor = new Sonar(DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);
    
    systemState = SystemState::OFFLINE;
}

Led *WaterLevelMonitor::getLed1() {
    return led1;
}

Led *WaterLevelMonitor::getLed2() {
    return led2;
}

int WaterLevelMonitor::getDistance() {
    return distanceSensor->getDistance();
}

SystemState WaterLevelMonitor::getState() {
    return systemState;
}

void WaterLevelMonitor::setState(SystemState newState) {
    systemState = newState;
}

int WaterLevelMonitor::getCarWashed() {
    return carWashed;
}