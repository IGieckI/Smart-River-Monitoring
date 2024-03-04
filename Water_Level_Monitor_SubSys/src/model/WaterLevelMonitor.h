#ifndef __WATERLEVELMONITOR__
#define __WATERLEVELMONITOR__

#include "Config.h"
#include "components/distantiometer/Sonar.h"
#include "components/light/Led.h"

enum SystemState {
    OFFLINE,
    ONLINE
};

class WaterLevelMonitor {
    private:
        SystemState systemState;

        Led *led1;
        Led *led2;
        DistanceSensor *distanceSensor;
        
        int carWashed;
    
    public:
        WaterLevelMonitor();
        Led *getLed1();
        Led *getLed2();
        int getDistance();
        SystemState getState();
        void setState(SystemState newState);
        int getCarWashed();
};

#endif
