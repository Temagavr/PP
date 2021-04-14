#include "CMyThread.h"

CRITICAL_SECTION CriticalSection;

DWORD WINAPI ThreadProc(const LPVOID lpParam)
{
    std::stringstream temp;
    int delta;
    delta = rand() % (10 - 1 + 1) + 1;
    EnterCriticalSection(&CriticalSection);
    int* value = (int*)lpParam;
    //int *param = (int*)lpParam;
    *value = *value + delta;
    *(int*)lpParam = *value;

    LeaveCriticalSection(&CriticalSection);
    temp << "value = " << *value << " param = " << *(int*)lpParam << " delta = " << delta << std::endl;
    std::cout << temp.str();

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int CMyThread::RunAllThreads()
{
    if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
        return 1;

    SetThreadAffinityMask(handles[0], 1);
    SetThreadAffinityMask(handles[1], 2);

    for (int i = 0; i < handles.size(); ++i)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы двух потоков
    WaitForMultipleObjects(handles.size(), handles.data(), true, INFINITE);

    DeleteCriticalSection(&CriticalSection);

    return 0;

}

void CMyThread::AddThread()
{
    handles.push_back(CreateThread(NULL, 0, &ThreadProc, (LPVOID)num, CREATE_SUSPENDED, NULL));
}
