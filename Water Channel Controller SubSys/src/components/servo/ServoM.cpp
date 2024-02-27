#include "ServoM.h"

ServoM::ServoM(unsigned short pin) : Component(new Pin(pin, OUTPUT), 1) { }