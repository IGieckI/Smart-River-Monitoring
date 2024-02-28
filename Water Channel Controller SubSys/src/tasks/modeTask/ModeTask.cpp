#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->state = AUTO;
    this->setName("ModeTask");
}

void ModeTask::tick() {
    switch (state)
    {
    case State::MANUAL:
        
        if (sys->isManuelMode()) {
            state = State::AUTO;
        }    
        break;
    case State::AUTO:

        if (sys->isManuelMode()) {
            state = State::MANUAL;
        }
        break;
    default:
        break;
    }
}