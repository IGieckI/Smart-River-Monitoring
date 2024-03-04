#include "tasks/modeSetterTask/ModeSetterTask.h"

ModeSetterTask::ModeSetterTask(int period, SubSys *sys) : Task(period, sys) {}

void ModeSetterTask::tick() {
    if (this->sys->isButtonPressed()) {
        if (!this->sys->isButtonPressed()) {
            this->sys->changeMode();
        }
    }
}