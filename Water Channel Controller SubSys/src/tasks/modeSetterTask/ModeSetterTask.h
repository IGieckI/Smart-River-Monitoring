#ifndef _MODE_SETTER_TASK_H_
#define _MODE_SETTER_TASK_H_

#include "tasks/Task.h"

class ModeSetterTask : public Task {
    private:
        bool btnPressed;
    public:
        ModeSetterTask(int period, SubSys *sys);
        void tick();
};

#endif