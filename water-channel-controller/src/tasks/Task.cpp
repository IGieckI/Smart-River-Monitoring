#include "tasks/Task.h"

Task::Task(int period, SubSys *sys) {
	Task::sys = sys;
	myPeriod = period;
	timeElapsed = 0;
}

bool Task::updateAndCheckTime(int basePeriod){
	timeElapsed += basePeriod;
	if (timeElapsed >= myPeriod){
		timeElapsed = 0;
		return true;
	} else {
		return false; 
	}
}

const char* Task::getName() {
	return name;
}

void Task::setName(const char* name) {
	this->name = name;
}