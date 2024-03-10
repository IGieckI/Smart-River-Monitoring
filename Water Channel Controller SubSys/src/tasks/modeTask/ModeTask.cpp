#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = AUTO;
    this->setName("ModeTask");
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->displayText("AUTO");
    MsgService.init();
    this->sys->getServoMotor()->setPosition(CLOSE_GATE_DEGREE);
    
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
    switch (modeState)
    {
    case ModeState::MANUAL : 
        if (sys->isManuelMode() == false) {
            modeState = ModeState::AUTO;
            this->sys->getLcd()->clearScreen();
            this->sys->getLcd()->displayText("AUTO");
            Serial.println("AUTO");
        }    
        break;
    case ModeState::AUTO :
        if (MsgService.isMsgAvailable()) {
            Msg* msg = MsgService.receiveMsg();    
            deserializeJson(doc, msg->getContent());
            int valveValue = doc[String("valve")];
            // showOnScreen("AUTO", doc["valve"]);
            // map the valveValue from 0-100 to 70-180
            Serial.println(String(valveValue).c_str());
            valveValue = map(valveValue, 100, 0, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            Serial.println(String(valveValue).c_str());
            this->sys->getServoMotor()->setPosition(valveValue);
            delete msg;
        }

        if (sys->isManuelMode() == true) {
            modeState = ModeState::MANUAL;
            this->sys->getLcd()->clearScreen();
            this->sys->getLcd()->displayText("MANUAL");
            Serial.println("MANUAL");
        }
        break;
    default:
        break;
    }
}