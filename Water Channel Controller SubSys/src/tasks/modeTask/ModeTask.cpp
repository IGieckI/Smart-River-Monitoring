#include "tasks/modeTask/ModeTask.h"

ModeTask::ModeTask(int period, SubSys *sys) : Task(period, sys) {
    this->modeState = ModeState::MANUAL;
    sys->changeMode();
}

void ModeTask::tick() {
    switch (modeState)
    {
    case ModeState::MANUAL : {
        sys->getGreenLed()->switchLight(false);

        int potValue = this->sys->getPotentiometer()->getValue();

        sys->setValveOpening(potValue);
        
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
        }    
    }
    break;
    case ModeState::AUTO : {
        sys->getGreenLed()->switchLight(true);

        if (Serial.availableForWrite())
        {
            sendJson();
        }
        
        if (Serial.available() > 0)
        {
            sys->getRedLed()->switchLight(true);
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

            sys->setValveOpening(valveValue);

            int valveValueMapped = map(valveValue, 0, 100, CLOSE_GATE_DEGREE, OPEN_GATE_DEGREE);
            this->sys->getServoMotor()->setPosition(valveValueMapped);
        } else {
            sys->getRedLed()->switchLight(false);
        }
        
        if (sys->isManuelMode() == true) {
            modeState = ModeState::MANUAL;
        }
    }
    break;
    default:
        break;
    }
}

void ModeTask::sendJson() {
    bool state = this->sys->isManuelMode();
    uint8_t valvePos = this->sys->getServoMotor()->getPosition() * 100 / 170;

    StaticJsonDocument<200> doc;
    doc["manual_control"] = state;
    doc["valve"] = valvePos;

    char jsonString[200]; // Allocate a buffer for the JSON string
    serializeJson(doc, jsonString);

    Serial.println(jsonString);
    Serial.flush();
}