#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>

clock_t t;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    std::stringstream temp;
    for (int i = 0; i < 20; ++i)
    {
        temp << (int)lpParam << " | " << (double)((clock() - t)) << " ; "; // /CLOCKS_PER_SEC можно добавить для вычисления секунд
        std::cout << temp.str();
        temp.str("");
    }
    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{

    setlocale(LC_ALL, "Russian");
    int threadCount = 2;
    
    // создание двух потоков
    HANDLE* handles = new HANDLE[threadCount];

    int num = 0;
    for (int i = 0; i < threadCount; ++i)
    {
        ++num;
        handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)num, CREATE_SUSPENDED, NULL);
    }

    SetThreadAffinityMask(handles[0], 1);
    SetThreadAffinityMask(handles[1], 1);

    for (int i = 0; i < threadCount; ++i)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы двух потоков
    WaitForMultipleObjects(threadCount, handles, true, INFINITE);
    return 0;
}
