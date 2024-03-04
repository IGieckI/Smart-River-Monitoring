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

        /**
         * The user can change the mode by pressing the button.
         * The state will change effectively when the button is released.
        */
        void tick();
};

#endif