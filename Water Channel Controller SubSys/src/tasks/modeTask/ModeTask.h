#ifndef _MODE_TASK_H_
#define _MODE_TASK_H_

#include "tasks/Task.h"
#include <ArduinoJson.h>

class ModeTask : public Task {
    private:
        enum ModeState 
        {
            MANUAL,
            AUTO
        };
        ModeState modeState;

        JsonDocument doc;

        /**
         * @brief Display the valve opening value on the lcd.
        */
        void ModeTask::displayInfoOnLcd(uint8_t val);
    public:
        ModeTask(int period, SubSys *sys);
        void tick();
};

#endif