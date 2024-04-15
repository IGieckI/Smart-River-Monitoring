#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = ModeState::AUTO;
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->setPosition(0, 0);
    this->sys->getLcd()->displayText("AUTO");
    MsgService.init();
}

void ModeTask::tick() {
    switch (modeState)
    {
    case ModeState::MANUAL : {
        digitalWrite(10, LOW);
        int potValue = this->sys->getPotentiometer()->getValue();
        
        /* set valve position */
        int valveValue = map(potValue, 0, 100, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);


        /* set gate opening */
        this->sys->getServoMotor()->setPosition(valveValue);

        if (Serial.availableForWrite())
        {
            sendJson();
        }

        if (sys->isManuelMode() == false) {
            modeState = ModeState::AUTO;
            this->sys->getLcd()->clearScreen();
            this->sys->getLcd()->setPosition(0, 0);
            this->sys->getLcd()->displayText("AUTO");
        }    
    }
    break;
    case ModeState::AUTO : {
        digitalWrite(10, HIGH);
        if (Serial.availableForWrite())
        {
            sendJson();
        }
        
        if (Serial.available() > 0)
        {
            digitalWrite(9, HIGH);
            String string = Serial.readStringUntil('\n');
            // Create a JSON buffer with enough capacity to hold the JSON object
            StaticJsonDocument<200> doc;

            // Parse the JSON string and handle parsing errors
            DeserializationError error = deserializeJson(doc, string);
            if (error) {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.f_str());
                return;
            }

            // Extract the integer value associated with the "valve" key
            int valveValue = doc["valve"];
            int valveValueMapped = map(valveValue, 0, 100, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            this->sys->getServoMotor()->setPosition(valveValueMapped);
        } else {
            digitalWrite(9, LOW);
        }
        
        if (sys->isManuelMode() == true) {
            modeState = ModeState::MANUAL;
            this->sys->getLcd()->clearScreen();
            this->sys->getLcd()->setPosition(0, 0);
            this->sys->getLcd()->displayText("MANUAL");
        }
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

void ModeTask::sendJson() {
    String state = this->sys->isManuelMode() ? "true" : "false";
    uint8_t valvePos = this->sys->getServoMotor()->getPosition();

    StaticJsonDocument<200> doc;
    doc["manual_control"] = state;
    doc["valve"] = valvePos;

    char jsonString[200]; // Allocate a buffer for the JSON string
    serializeJson(doc, jsonString);

    // sprintf(buffer, "{\"manual_control\":\"%s\",\"valve\":\"%d\"}", String(state).c_str(), valvePos);
    Serial.println(jsonString);
    Serial.flush();
}