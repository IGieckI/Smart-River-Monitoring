#include "Component.h"

Component::Component(Pin pins[], int numPins) {
    this->pins = new Pin[numPins];
    
    for (int i = 0; i < numPins; i++) {
        this->pins[i] = pins[i];
        pinMode(pins[i].getPinNumber(), pins[i].getPinType());
    }
}

Component::Component() {
}

Component::~Component() {
    delete[] pins;
}