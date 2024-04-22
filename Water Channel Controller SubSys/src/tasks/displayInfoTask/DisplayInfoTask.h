#ifndef __DISPLAY_INFO_TASK_H__
#define __DISPLAY_INFO_TASK_H__

#include "tasks/Task.h"

class DisplayInfoTask : public Task {
    private:
        void displayInfoOnLcd(char* mode, uint8_t val);
    public:
        DisplayInfoTask(int period, SubSys *sys);
        void tick();
};

#endif