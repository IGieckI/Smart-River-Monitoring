#include "PotentiometerImpl.h"

PotentiometerImpl::PotentiometerImpl(unsigned short pin) : Potentiometer(pin) { }

int PotentiometerImpl::getValue() {
    int value = map(analogRead(pins[0].getPinNumber()), 0, 1023, 0, 100);
}