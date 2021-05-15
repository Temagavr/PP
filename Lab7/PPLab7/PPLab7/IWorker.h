#pragma once
#include <windows.h>
#include <iostream>
#include "ITask.h"

class IWorker
{
public:
	virtual bool ExecuteTask(ITask* taskToRun) = 0;
	virtual bool IsBusy() = 0;
};
