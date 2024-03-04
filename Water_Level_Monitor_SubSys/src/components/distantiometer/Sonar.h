#ifndef __SONAR__
#define __SONAR__

#include "DistanceSensor.h"

class Sonar: public DistanceSensor {
    private:
        const float vs = 331.5 + 0.6*20;

    public:
        Sonar(unsigned short echoPin, unsigned short trigPin);
        float getDistance();
};

#endif
