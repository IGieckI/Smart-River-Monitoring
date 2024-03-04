#include "ButtonImpl.h"

ButtonImpl::ButtonImpl(unsigned short pin) : Button(pin) { }

bool ButtonImpl::isClicked() {
     if (millis() - lastClickTime > 200) {
          lastClickTime = millis();
          return digitalRead(this->pins[0].getPinNumber()) == HIGH;
     } else {
          return false;
     }
}
