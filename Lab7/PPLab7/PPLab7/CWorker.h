#pragma once
#include "IWorker.h"

class CWorker : IWorker
{
public:
	CWorker(){};

	~CWorker()
	{
		WaitForSingleObject(handle, INFINITE);
	};

	bool ExecuteTask(ITask* taskToRun) override;
	bool IsBusy() override;

private:
	HANDLE handle;

};
