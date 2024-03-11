#ifndef _POTEMTIOMETERIMPL_H_
#define _POTEMTIOMETERIMPL_H_

#include "Potentiometer.h"

class PotentiometerImpl : public Potentiometer {
    public:
        PotentiometerImpl(unsigned short pin);
        int getValue();
};

#endif