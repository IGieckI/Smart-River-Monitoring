#include "Pin.h"

Pin::Pin(unsigned short pinNumber, unsigned short type) {
    this->pinNumber = pinNumber;
    this->type = type;
}

Pin::Pin() {
}

int Pin::getPinNumber() {
    return this->pinNumber;
}

unsigned short Pin::getPinType() {
    return this->type;
}