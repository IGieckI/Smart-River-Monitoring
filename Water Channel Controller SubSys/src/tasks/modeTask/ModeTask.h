#ifndef _MODE_TASK_H_
#define _MODE_TASK_H_

#include "tasks/Task.h"

class ModeTask : public Task {
    private:
        enum State 
        {
            MANUAL,
            AUTO
        };
        State state;
    public:
        ModeTask(int period, SubSys *sys);
        void tick();
};

#endif