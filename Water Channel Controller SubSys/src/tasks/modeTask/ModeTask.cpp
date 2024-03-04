#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->state = AUTO;
    this->setName("ModeTask");
}

void ModeTask::tick() {
    switch (state)
    {
    case State::MANUAL: 
        Serial.println("MANUAL");
        if (sys->isManuelMode() == false) {
            state = State::AUTO;
        }    
        break;
    case State::AUTO:
        Serial.println("AUTO");
        if (sys->isManuelMode() == true) {
            state = State::MANUAL;
        }
        break;
    default:
        break;
    }
}