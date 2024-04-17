#ifndef __SONAR__
#define __SONAR__

#include "DistanceSensor.h"

class Sonar: public DistanceSensor {
    private:
        const float speed_velocity = 0.034 / 2;

    public:
        Sonar(unsigned short echoPin, unsigned short trigPin, long timeOut);
        float getDistance();
};

#endif
