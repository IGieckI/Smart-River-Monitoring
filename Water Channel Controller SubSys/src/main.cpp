#include <Arduino.h> 
#include "Config.h"
#include "Scheduler.h"
#include "model/SubSys.h"
#include "tasks/modeTask/ModeTask.h"
#include "tasks/modeSetterTask/ModeSetterTask.h"
#include "tasks/displayInfoTask/DisplayInfoTask.h"

Scheduler scheduler;
SubSys *sys;

void setup() {
    Serial.begin(115200);

    sys = new SubSys();
    scheduler.init(100);

    Task *modeSetterTask = new ModeSetterTask(100, sys);
    modeSetterTask->setName("ModeSetterTask");
    scheduler.addTask(modeSetterTask);

    Task *modeTask = new ModeTask(100, sys);
    modeTask->setName("ModeTask");
    scheduler.addTask(modeTask);

    Task *displayInfoTask = new DisplayInfoTask(750, sys);
    displayInfoTask->setName("DisplayInfoTask");
    scheduler.addTask(displayInfoTask);
}

void loop() {
    scheduler.schedule();
}
