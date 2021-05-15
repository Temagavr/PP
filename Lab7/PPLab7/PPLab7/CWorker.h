#pragma once
#include "IWorker.h"
#include <vector>

class CWorker : IWorker
{
public:
	CWorker();

	~CWorker();

	bool ExecuteTask(ITask* taskToRun) override;
	bool IsBusy() override;

private:
	std::vector<HANDLE> handles;
	bool isBisy = false;
	bool stopWorkingFlag = false;

};
