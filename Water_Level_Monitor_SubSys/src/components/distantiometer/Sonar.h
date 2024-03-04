#ifndef __SONAR__
#define __SONAR__

#include "DistanceSensor.h"

#define NO_OBJ_DETECTED -1

class Sonar: public DistanceSensor {
    private:
        const float vs = 331.5 + 0.6*20;

    public:
        Sonar(unsigned short echoPin, unsigned short trigPin);
        float getDistance();
};

#endif
