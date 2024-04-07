#ifndef __WATERLEVELMONITOR__
#define __WATERLEVELMONITOR__

#include "components/distantiometer/Sonar.h"
#include "components/light/Led.h"

class WaterLevelMonitor {
    private:
        Led *greenLed;
        Led *redLed;
        DistanceSensor *distanceSensor;
    
    public:
        WaterLevelMonitor(uint8_t redLedPin, uint8_t greenLedPin, uint8_t echoPin, uint8_t trigPin, int maxDistanceTime);
        void switchRedLed(bool state);
        void switchGreenLed(bool state);
        unsigned short getWaterLevel(int minDistance);
};

#endif
