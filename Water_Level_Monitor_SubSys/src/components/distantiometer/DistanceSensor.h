#ifndef __DISTANCE_SENSOR__
#define __DISTANCE_SENSOR__

#include "../Component.h"

class DistanceSensor : public Component {
public:
    DistanceSensor(Pin pins[], int numPins);
    virtual float getDistance() = 0;
};

#endif
