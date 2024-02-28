#include "Scheduler.h"

Timer* Timer_1;

void Scheduler::init(int basePeriod){
    Timer_1 = new Timer();
    this->basePeriod = basePeriod;
    Timer_1->setupPeriod(basePeriod);
    nTasks = 0;
}

bool Scheduler::addTask(Task* task){
    if (nTasks < MAX_TASKS-1){
        this->taskList[nTasks] = task;
        this->nTasks++;
        return true;
    } else {
        return false; 
    }
}
  
void Scheduler::schedule(){    
    Timer_1->waitForNextTick();

    for (int i = 0; i < nTasks; i++){
        if (taskList[i]->updateAndCheckTime(basePeriod)){
            taskList[i]->tick();
        }
    }
}
