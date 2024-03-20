#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = AUTO;
    this->setName("ModeTask");
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->setPosition(0, 0);
    this->sys->getLcd()->displayText("AUTO");
    MsgService.init();
    this->sys->getServoMotor()->setPosition(CLOSE_GATE_DEGREE);
}

void ModeTask::tick() {
    switch (modeState)
    {
    case ModeState::MANUAL : {
            int potValue = this->sys->getPotentiometer()->getValue();
            
            /* set valve position */
            int valveValue = map(potValue, 0, 100, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            Serial.println(valveValue);

            /* show on lcd valve openening value */
            this->sys->getLcd()->setPosition(2, 0);
            this->sys->getLcd()->displayText("Valve:");
            this->sys->getLcd()->setPosition(2, 6);
            this->sys->getLcd()->displayText("    ");
            this->sys->getLcd()->setPosition(2, 6);
            this->sys->getLcd()->displayText(String(potValue).c_str());

            Serial.println(valveValue);
            this->sys->getServoMotor()->setPosition(valveValue);


            if (sys->isManuelMode() == false) {
                modeState = ModeState::AUTO;
                this->sys->getLcd()->setPosition(0, 0);
                this->sys->getLcd()->displayText("AUTO");
                Serial.println("AUTO");
            }    
        }
        break;
    case ModeState::AUTO :
        Serial.println("auto");
        if (MsgService.isMsgAvailable()) {
            Msg* msg = MsgService.receiveMsg();    
            deserializeJson(doc, msg->getContent());
            int valveValue = doc[String("valve")];
            Serial.println(String(valveValue).c_str());
            
            /* show on lcd valve openening value */
            this->sys->getLcd()->setPosition(2, 0);
            this->sys->getLcd()->displayText("Valve:");
            this->sys->getLcd()->setPosition(2, 6);
            this->sys->getLcd()->displayText("    ");
            this->sys->getLcd()->setPosition(2, 6);
            this->sys->getLcd()->displayText(String(valveValue).c_str());

            /* set valve position */
            valveValue = map(valveValue, 0, 100, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            this->sys->getServoMotor()->setPosition(valveValue);
            delete msg;
        }

        if (sys->isManuelMode() == true) {
            modeState = ModeState::MANUAL;
            this->sys->getLcd()->setPosition(0, 0);
            this->sys->getLcd()->displayText("    ");
            this->sys->getLcd()->setPosition(0, 0);
            this->sys->getLcd()->displayText("MANUAL");
            Serial.println("MANUAL");
        }
        break;
    default:
        break;
    }
}