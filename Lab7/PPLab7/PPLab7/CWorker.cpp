#include "CWorker.h"

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ITask *task = (ITask*)lpParam;

	task->Execute();
	ExitThread(0);
}

bool CWorker::ExecuteTask(ITask* task)
{
	if (IsBusy())
	{
		std::cout << "Thread is busy\n";
		return false;
	}
	if (handle = CreateThread(NULL, 0, &ThreadProc, task, CREATE_SUSPENDED, NULL))
	{
		ResumeThread(handle);
		return true;
	}
	return false;
}

bool CWorker::IsBusy()
{
	DWORD exitcode;
	GetExitCodeThread(handle, &exitcode);
	if (exitcode == STILL_ACTIVE)
		return true;
	return false;
}
