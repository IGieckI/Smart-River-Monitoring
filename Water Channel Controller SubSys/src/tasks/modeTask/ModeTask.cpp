#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = AUTO;
    this->setName("ModeTask");
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->displayText("AUTO");
    MsgService.init();
    this->sys->getServoMotor()->setPosition(CLOSE_GATE_DEGREE);
    
}

void ModeTask::tick() {
    switch (modeState)
    {
    case ModeState::MANUAL : 

        int potValue = this->sys->getPotentiometer()->getValue();
        // Serial.println(potValue);
        // int valveValue = map(potValue, 0, 100, OPEN_GATE_DEGREE, CLOSE_GATE_DEGREE);
        // Serial.println(valveValue);
        // this->sys->getServoMotor()->setPosition(valveValue);

        if (sys->isManuelMode() == false) {
            modeState = ModeState::AUTO;
            this->sys->getLcd()->clearScreen();
            this->sys->getLcd()->displayText("AUTO");
            Serial.println("AUTO");
        }    
        break;
    case ModeState::AUTO :
        Serial.println("auto");
        if (MsgService.isMsgAvailable()) {
            Msg* msg = MsgService.receiveMsg();    
            deserializeJson(doc, msg->getContent());
            int valveValue = doc[String("valve")];
            Serial.println(String(valveValue).c_str());
            valveValue = map(valveValue, 100, 0, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            Serial.println(String(valveValue).c_str());
            this->sys->getServoMotor()->setPosition(valveValue);
            
            /* show on lcd valve openening value*/
            this->sys->getLcd()->newLine();
            this->sys->getLcd()->displayText(String(valveValue).c_str());
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