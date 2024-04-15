#ifndef _MODE_TASK_H_
#define _MODE_TASK_H_

#include "tasks/Task.h"
#include <ArduinoJson.h>

class ModeTask : public Task {
    private:
        String string;
        enum ModeState 
        {
            MANUAL,
            AUTO
        };
        ModeState modeState;

        JsonDocument doc;

        void sendJson();

        /**
         * @brief Display the valve opening value on the lcd.
        */
        void displayInfoOnLcd(uint8_t val);
    public:
        ModeTask(int period, SubSys *sys);
        void tick();
};

#endif