#ifndef _POTEMTIOMETERIMPL_H_
#define _POTEMTIOMETERIMPL_H_

#include "Potentiometer.h"

class PotentiometerImpl : public Potentiometer {
    public:
        PotentiometerImpl(unsigned short pin);
        
        /**
         * Return the potentiometer value in range 0-100.
        */
        virtual int getValue();
};

#endif