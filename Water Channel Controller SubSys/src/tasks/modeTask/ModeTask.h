#ifndef _MODE_TASK_H_
#define _MODE_TASK_H_

#include "tasks/Task.h"

class ModeTask : public Task {
    private:
        enum ModeState 
        {
            MANUAL,
            AUTO
        };
        ModeState modeState;

        /**
         * Show on screen the mode and the water level.
         * @param mode the mode to show on screen.
         * @param waterLevel the water level to show on screen.
        */
        void showOnScreen(char *mode, float waterLevel);

        /**
         * Set the gate by the valve.
         * Using the potentiometer the user can change the gate opening.
         * This fun map the potentiometer value to the gate value.
        */
        void setGateByValve();
    public:
        ModeTask(int period, SubSys *sys);
        void tick();
};

#endif