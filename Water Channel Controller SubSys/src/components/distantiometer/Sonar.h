#ifndef __SONAR__
#define __SONAR__

#include "DistanceSensor.h"

#define NO_OBJ_DETECTED -1

class Sonar: public DistanceSensor {
    private:
        Sonar(Pin pins[], int numPins);
        const float vs = 331.5 + 0.6*20;
        long timeOut;

    public:
        Sonar(unsigned short echoPin, unsigned short trigPin, long maxTime);
        float getDistance();
        void setTemperature(float temp);
};

#endif
