#ifndef __SONAR__
#define __SONAR__

#include "DistanceSensor.h"

class Sonar: public DistanceSensor {
    private:
        const float DISTANCE_CALCULATION_CONSTANT = 0.0343 / 2;

    public:
        Sonar(unsigned short echoPin, unsigned short trigPin, long timeOut);
        float getDistance();
};

#endif
