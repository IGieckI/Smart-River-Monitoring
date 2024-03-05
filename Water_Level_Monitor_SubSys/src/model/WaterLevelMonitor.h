#ifndef __WATERLEVELMONITOR__
#define __WATERLEVELMONITOR__

#include "Config.h"
#include "components/distantiometer/Sonar.h"
#include "components/light/Led.h"

class WaterLevelMonitor {
    private:
        Led *greenLed;
        Led *redLed;
        DistanceSensor *distanceSensor;
        
        int carWashed;
    
    public:
        WaterLevelMonitor();
        Led *getGreenLed();
        Led *getRedLed();
        int getWaterLevel();
};

#endif
