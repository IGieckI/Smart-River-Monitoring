#include "ServoMImpl.h"

ServoMImpl::ServoMImpl(unsigned short pin) : ServoM(pin) {
    servo.attach(pin);
    angle = CLOSE_GATE_DEGREE;
    setPosition(angle);
}

void ServoMImpl::setPosition(int _angle) {
    angle = _angle;
    if (angle > OPEN_GATE_DEGREE) {
        angle = OPEN_GATE_DEGREE;
    } else if (angle < CLOSE_GATE_DEGREE) {
        angle = CLOSE_GATE_DEGREE;
    }

    float coeff = (2400.0 - 544.0) / 180;
    servo.write(544 + angle * coeff);
}

int ServoMImpl::getPosition() {
    return angle;
}

bool ServoMImpl::isClose() {
    return angle >= CLOSE_GATE_DEGREE;
}

bool ServoMImpl::isOpen() {
    return angle == OPEN_GATE_DEGREE;
}

int ServoMImpl::getDefaultCloseAngle() {
    return CLOSE_GATE_DEGREE;
}

int ServoMImpl::getDefaultOpenAngle() {
    return OPEN_GATE_DEGREE;
}