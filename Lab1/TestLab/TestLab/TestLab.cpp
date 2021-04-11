#include <windows.h>
#include <string>
#include <iostream>
#include <sstream>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    //
    // ?? тут выполняется полезная работа потока
    //
    std::stringstream temp;
    temp << "Поток №" << (int)lpParam << " выполняется" << std::endl;
    std::cout << temp.str();

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
    int threadCount = std::stoi(argv[1]);
    //std::cout << argv[1] << std::endl;
    // создание двух потоков
    HANDLE* handles = new HANDLE[threadCount];

    int num = 0;
    for (int i = 0; i < threadCount; ++i)
    {
        ++num;
        handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)num, CREATE_SUSPENDED, NULL);
    }

    for (int i = 0; i < threadCount; ++i)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы двух потоков
    WaitForMultipleObjects(threadCount, handles, true, INFINITE);
    return 0;
}
