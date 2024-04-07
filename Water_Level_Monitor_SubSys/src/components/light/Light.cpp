#include "Light.h"

Light::Light(unsigned short pin) : Component(new Pin(pin, OUTPUT), 1) { }