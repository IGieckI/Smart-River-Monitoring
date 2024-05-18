#include "components/potentiometer/Potentiometer.h"

Potentiometer::Potentiometer(unsigned short pin) : Component(new Pin(pin, INPUT), 1) { }