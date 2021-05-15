#include "CWorker.h"

CRITICAL_SECTION CriticalSection;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ITask *task = (ITask*)lpParam;

	EnterCriticalSection(&CriticalSection);
	task->Execute();
	LeaveCriticalSection(&CriticalSection);
	ExitThread(0);
}

CWorker::CWorker()
{
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
	{}
}

CWorker::~CWorker()
{
	stopWorkingFlag = true;
	if (!handles.empty())
	{
		WaitForSingleObject(handles.front(), INFINITE);
		for (auto i = handles.begin(); i != handles.end(); i++)
		{
			TerminateThread(*i, 1);
		}
	}
	DeleteCriticalSection(&CriticalSection);
}

bool CWorker::ExecuteTask(ITask* task)
{
	if (stopWorkingFlag)
		return false;

	HANDLE newHandle = CreateThread(NULL, 0, &ThreadProc, (LPVOID)(task), CREATE_SUSPENDED, NULL);

	handles.push_back(newHandle);
	if (handles.size() == 1)
		ResumeThread(handles.front());

	WaitForSingleObject(handles.back(), INFINITE);

	if (handles.size() > 0 && !stopWorkingFlag)
		ResumeThread(handles.front());

	handles.erase(handles.begin());

	if(stopWorkingFlag)
		return false;

	return true;
}

bool CWorker::IsBusy()
{
	return handles.size() > 0;
}
