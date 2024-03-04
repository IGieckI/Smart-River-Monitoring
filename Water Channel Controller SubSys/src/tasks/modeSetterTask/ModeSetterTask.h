#ifndef _MODE_SETTER_TASK_H_
#define _MODE_SETTER_TASK_H_

#include "tasks/Task.h"

class ModeSetterTask : public Task {
    private:
        enum ButtonState {
            NOT_PRESSED,
            IN_PRESSING,
            PRESSED
        };
        bool btnState;
        ButtonState buttonState;
    public:
        ModeSetterTask(int period, SubSys *sys);
        void tick();
};

#endif