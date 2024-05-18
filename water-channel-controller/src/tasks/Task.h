#ifndef __TASK_H__
#define __TASK_H__

#include "model/SubSys.h"

class Task {
	protected:
		SubSys *sys;
		int myPeriod;
		int timeElapsed;
		const char* name;
	
	public:
		Task(int period, SubSys *sys);

		virtual void tick() = 0;

		bool updateAndCheckTime(int basePeriod);

		const char* getName();

		void setName(const char* name);
  
};

#endif
