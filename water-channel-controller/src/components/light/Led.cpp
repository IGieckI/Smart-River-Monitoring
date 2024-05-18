#include "Led.h"

Led::Led(unsigned short pin) : Light(pin) { }

void Led::switchLight(bool state) {
    digitalWrite(this->pins[0].getPinNumber(), state ? HIGH : LOW);
}

bool Led::isOn() {
    return digitalRead(this->pins[0].getPinNumber()) == HIGH;
}

void Led::fading() {
    if ((currentLight >= 255 && fadeState) || (currentLight <= 0 && !fadeState)) {
        fadeState = !fadeState;
    }
    currentLight += fadeState ? FADE_STEP : -FADE_STEP;
    analogWrite(this->pins[0].getPinNumber(), currentLight);
}

void Led::blink(unsigned long time) {
    if (millis() - this->lastBlink >= time)
    {
        if (this->isOn())
        {
            this->switchLight(false);
        } else {
            this->switchLight(true);
        }
        this->lastBlink = millis();
    }
    
}
