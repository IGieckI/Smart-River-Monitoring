#include <Arduino.h>
#include "config.h"
#include "Scheduler.h"

Scheduler *scheduler;

void setup() {
    
}

void loop() {
    scheduler->schedule();
}
