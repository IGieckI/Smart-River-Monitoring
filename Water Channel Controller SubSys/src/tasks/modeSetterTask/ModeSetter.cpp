#include "tasks/modeSetterTask/ModeSetterTask.h"

ModeSetterTask::ModeSetterTask(int period, SubSys *sys) : Task(period, sys) {
    this->btnState = ButtonState::NOT_PRESSED;
}

void ModeSetterTask::tick() {
    switch (btnState)
    {
    case ButtonState::NOT_PRESSED:

        if (this->sys->isButtonPressed()) {
            btnState = ButtonState::IN_PRESSING;
        }
        break;
    case ButtonState::IN_PRESSING:
        if (!this->sys->isButtonPressed()) {
            btnState = ButtonState::PRESSED;
        }
        break;
    case ButtonState::PRESSED:
            this->sys->changeMode();
            btnState = ButtonState::NOT_PRESSED;
        break;
    default:
        break;
    }
}