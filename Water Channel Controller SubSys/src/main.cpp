#include <Arduino.h>
#include "Config.h"
#include "Scheduler.h"
#include "model/SubSys.h"
#include "tasks/modeTask/ModeTask.h"
#include "tasks/modeSetterTask/ModeSetterTask.h"

Scheduler scheduler;
SubSys *sys;

void setup() {
    sys = new SubSys();
    Serial.begin(9600);
    scheduler.init(100);

    Task *modeSetterTask = new ModeSetterTask(50, sys);
    modeSetterTask->setName("ModeSetterTask");
    scheduler.addTask(modeSetterTask);

    Task *modeTask = new ModeTask(200, sys);
    modeTask->setName("ModeTask");
    scheduler.addTask(modeTask);
    // gen random value from 0 to 100
    randomSeed(analogRead(0));

}

void sendRandomDataForTesting() {
    // 0-100 % dell'apertura della valvola definita da remoto
    char dataOut[100];
    sprintf(dataOut, "{\"valve\": \"%d\"}", random(0, 100));
    Serial.println(dataOut);
}

void loop() {
    sendRandomDataForTesting();
    scheduler.schedule();
}
