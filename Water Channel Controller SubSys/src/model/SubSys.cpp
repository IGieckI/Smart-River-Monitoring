#include "model/SubSys.h"

SubSys::SubSys() {
    servoMotor = new ServoMImpl(SERVO_PIN);
    lcd = new LcdI2C(LCD_ADDR, LCD_COLS, LCD_ROWS, A4, A5);
    button = new ButtonImpl(BUTTON_PIN);
    potentiometer = new PotentiometerImpl(A0);
    manuelMode = false;
    this->greenLed = new Led(GREEN_LED_PIN);
    this->redLed = new Led(RED_LED_PIN);
}

bool SubSys::isButtonPressed() {
    return button->isClicked();
}

ServoM *SubSys::getServoMotor() {
    return servoMotor;
}

LcdI2C *SubSys::getLcd() {
    return lcd;
}

void SubSys::changeMode() {
    manuelMode = !manuelMode;
}

bool SubSys::isManuelMode() {
    return manuelMode;
}

PotentiometerImpl *SubSys::getPotentiometer() {
    return potentiometer;
}

void SubSys::setValveOpening(uint8_t val) {
    this->valveOpening = val;
}

uint8_t SubSys::getValveOpening() {
    return valveOpening;
}

Led *SubSys::getGreenLed() {
    return greenLed;
}

Led *SubSys::getRedLed() {
    return redLed;
}
