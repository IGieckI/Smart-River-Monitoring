# include "tasks/displayInfoTask/DisplayInfoTask.h"

DisplayInfoTask::DisplayInfoTask(int period, SubSys *sys) : Task(period, sys) {
    this->sys->getLcd()->clearScreen();
}

void DisplayInfoTask::tick() {
    if (sys->isManuelMode()) {
        displayInfoOnLcd("MANUAL", sys->getValveOpening());
    } else {
        displayInfoOnLcd("AUTO", sys->getValveOpening());
    }
}

void DisplayInfoTask::displayInfoOnLcd(char* mode, uint8_t val) {
    this->sys->getLcd()->clearScreen();
    this->sys->getLcd()->setPosition(0, 0);
    this->sys->getLcd()->displayText("MODE: ");
    this->sys->getLcd()->displayText(mode);
    this->sys->getLcd()->setPosition(1, 0);
    this->sys->getLcd()->displayText("VALVE: ");
    this->sys->getLcd()->displayText(String(val).c_str());
}