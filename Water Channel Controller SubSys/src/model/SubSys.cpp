#include "model/SubSys.h"

SubSys::SubSys() {
    distanceSensor = new Sonar(DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);
    servoMotor = new ServoMImpl(SERVO_PIN);
    lcd = new LcdI2C();
    button = new ButtonImpl(BTN_PIN);
}

int SubSys::getWaterDistance() {
    return distanceSensor->getDistance();
}

bool SubSys::isButtonPressed() {
    return button->isClicked();
}

ServoM *SubSys::getServoMotor() {
    return servoMotor;
}

Lcd *SubSys::getLcd() {
    return lcd;
}

void SubSys::changeMode() {
    manuelMode = !manuelMode;
}

bool SubSys::isManuelMode() {
    return manuelMode;
}