#include "tasks/modeSetterTask/ModeSetterTask.h"

ModeSetterTask::ModeSetterTask(int period, SubSys *sys) : Task(period, sys) {
    this->setName("ModeSetterTask");
}

void ModeSetterTask::tick() {
    if (this->sys->isButtonPressed()) {
        this->sys->changeMode();
    }
}