#ifndef _GATE_TASK_H_
#define _GATE_TASK_H_

#include "tasks/Task.h"
#include <Arduino.h>

class GateTask : public Task {
    private:
        enum State {
            OPENING,
            CLOSING,
            OPEN,
            CLOSED
        };
        State state;
    public:
        GateTask(int period, SubSys *sys);
        void tick();
};

#endif