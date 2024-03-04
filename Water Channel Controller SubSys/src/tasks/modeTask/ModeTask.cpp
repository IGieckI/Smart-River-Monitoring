#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->state = AUTO;
    this->setName("ModeTask");
}

void ModeTask::setGateByValve() {
    /* TO DO */
}

void ModeTask::showOnScreen(char *mode, float waterLevel) {
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->displayText(mode);
    this->sys->getLcd()->newLine();
    this->sys->getLcd()->displayText("WaterLevel: ");
    this->sys->getLcd()->displayText(String(waterLevel).c_str());
}

void ModeTask::tick() {
    switch (state)
    {
    case State::MANUAL: 
        // Serial.println("MANUAL");
        showOnScreen("MANUAL MODE", this->sys->getWaterDistance());
        setGateByValve();
        if (sys->isManuelMode() == false) {
            state = State::AUTO;
        }    
        break;
    case State::AUTO:
        showOnScreen("AUTO MODE", this->sys->getWaterDistance());
        // Serial.println("AUTO");
        if (MsgService.isMsgAvailable()) {
            Msg* msg = MsgService.receiveMsg();    
            if (msg->getContent() == "TO DO"){
                /* TO DO */
            }
            delete msg;
        }

        if (sys->isManuelMode() == true) {
            state = State::MANUAL;
        }
        break;
    default:
        break;
    }
}