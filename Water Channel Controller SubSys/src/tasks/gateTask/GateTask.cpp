#include "GateTask.h"

GateTask::GateTask(int period, SubSys *sys) : Task(period, sys) {
    this->state = CLOSED;
    this->setName("GateTask");
}

void GateTask::tick() {
    switch (state)
    {
    case State::OPENING:
        /* code */
        break;
    case State::CLOSING:
        sys->getServoMotor()->setPosition(0);
        break;
    case State::OPEN:
        sys->getServoMotor()->setPosition(180);
        break;
    case State::CLOSED:
    default:
        break;
    }
}