#include "model/SubSys.h"

SubSys::SubSys() {
    distanceSensor = new Sonar(DISTANCE_SENSOR_ECHO_PIN, DISTANCE_SENSOR_TRIG_PIN, MAX_DISTANCE_TIME);
    servoMotor = new ServoMImpl(SERVO_PIN);
    lcd = new LcdI2C(LCD_ADDR, 16, 2, A4, A5);
    button = new ButtonImpl(7);
    potentiometer = new PotentiometerImpl(A0);
    manuelMode = false;
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

Potentiometer *SubSys::getPotentiometer() {
    return potentiometer;
}