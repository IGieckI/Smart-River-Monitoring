#ifndef COMPONENT_H
#define COMPONENT_H

#include "Pin.h"
#include <Arduino.h>

class Component {
    protected:
        Pin *pins;
        
    public:
        Component(Pin pins[], int numPins);
        Component();
        ~Component();
};

#endif
