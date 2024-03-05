#include "tasks/modeSetterTask/ModeSetterTask.h"

ModeSetterTask::ModeSetterTask(int period, SubSys *sys) : Task(period, sys) {
    this->buttonState = ButtonState::NOT_PRESSED;
}

void ModeSetterTask::tick() {
    switch (buttonState)
    {
    case ButtonState::NOT_PRESSED:
        Serial.println("Button not pressed");
        if (this->sys->isButtonPressed()) {
            buttonState = ButtonState::IN_PRESSING;
        }
        break;
    case ButtonState::IN_PRESSING:
        Serial.println("Button in pressing");
        if (!this->sys->isButtonPressed()) {
            buttonState = ButtonState::PRESSED;
        }
        break;
    case ButtonState::PRESSED:
        Serial.println("Button pressed");
        this->sys->changeMode();
        buttonState = ButtonState::NOT_PRESSED;
        break;
    default:
        break;
    }
}