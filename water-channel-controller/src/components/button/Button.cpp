#include "Button.h"

Button::Button(unsigned short pin) : Component(new Pin(pin, INPUT), 1) { }
