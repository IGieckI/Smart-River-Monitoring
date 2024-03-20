#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = AUTO;
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

            /* show on lcd valve openening value */
            displayInfoOnLcd(potValue);

            /* set gate opening */
            this->sys->getServoMotor()->setPosition(valveValue);

            char buffer[100];
            String state = this->sys->isManuelMode() ? "true" : "false";
            uint8_t valvePos = map(this->sys->getServoMotor()->getPosition(), CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE, 0, 100);
            sprintf(buffer, "{ \"manual_control\": \"%s\", \"valve\": \"%d\" }", String(state).c_str(), valvePos);
            Serial.println(buffer);


            if (sys->isManuelMode() == false) {
                modeState = ModeState::AUTO;
                this->sys->getLcd()->clearScreen();
                this->sys->getLcd()->setPosition(0, 0);
                this->sys->getLcd()->displayText("AUTO");
            }    
        }
        break;
    case ModeState::AUTO :
        if (MsgService.isMsgAvailable()) {
            Msg* msg = MsgService.receiveMsg();    
            deserializeJson(doc, msg->getContent());
            int valveValue = doc["valve"];
            
            /* show on lcd valve openening value */
            displayInfoOnLcd(valveValue);

            /* set valve position */
            valveValue = map(valveValue, 0, 100, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            this->sys->getServoMotor()->setPosition(valveValue);
            delete msg;
        }

        if (sys->isManuelMode() == true) {
            modeState = ModeState::MANUAL;
            this->sys->getLcd()->clearScreen();
            this->sys->getLcd()->setPosition(0, 0);
            this->sys->getLcd()->displayText("MANUAL");
        }
        break;
    default:
        break;
    }
}

void ModeTask::displayInfoOnLcd(uint8_t val) {
    this->sys->getLcd()->setPosition(2, 0);
    this->sys->getLcd()->displayText("Valve:");
    this->sys->getLcd()->displayText(String(val).c_str());
    this->sys->getLcd()->displayText("%");
}