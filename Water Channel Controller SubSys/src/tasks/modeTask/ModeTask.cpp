#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = AUTO;
    this->setName("ModeTask");
}

void ModeTask::setGateByValve() {
    /* TO DO */
}

void ModeTask::showOnScreen(char *mode, float waterLevel) {
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->displayText(mode);
    // this->sys->getLcd()->newLine();
    // this->sys->getLcd()->displayText("WaterLevel: ");
    // this->sys->getLcd()->displayText(String(waterLevel).c_str());
}

void ModeTask::tick() {
    switch (modeState)
    {
    case ModeState::MANUAL : 
        this->sys->getLcd()->clearScreen();
        this->sys->getLcd()->displayText("MANUAL");
        // setGateByValve();
        if (sys->isManuelMode() == false) {
            modeState = ModeState::AUTO;
        }    
        break;
    case ModeState::AUTO :
        this->sys->getLcd()->clearScreen();
        this->sys->getLcd()->displayText("AUTO");
        // if (MsgService.isMsgAvailable()) {
        //     Msg* msg = MsgService.receiveMsg();    
        //     if (msg->getContent() == "TO DO"){
        //         /* TO DO */
        //     }
        //     delete msg;
        // }
        if (sys->isManuelMode() == true) {
            modeState = ModeState::MANUAL;
        }
        break;
    default:
        break;
    }
}