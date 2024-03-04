#include <Arduino.h>
#include "Config.h"
#include "Scheduler.h"
#include "model/SubSys.h"
#include "tasks/modeTask/ModeTask.h"
#include "tasks/modeSetterTask/ModeSetterTask.h"

Scheduler *scheduler;
SubSys *sys;

void setup() {
    sys = new SubSys();
    Serial.begin(9600);
    scheduler = new Scheduler();
    scheduler->init(100);

    Task *modeSetterTask = new ModeSetterTask(100, sys);
    modeSetterTask->setName("ModeSetterTask");
    scheduler->addTask(modeSetterTask);

    Task *modeTask = new ModeTask(100, sys);
    modeTask->setName("ModeTask");
    scheduler->addTask(modeTask);

}

void loop() {
    scheduler->schedule();
}
