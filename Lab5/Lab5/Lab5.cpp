#include <windows.h>
#include <string>
#include <iostream>
#include "CMyThread.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");
   
    CMyThread handle;

    handle.AddThread();
    handle.AddThread();

    if (handle.RunAllThreads() == 1)
    {
        std::cout << "Error with critical section" << std::endl;
        return 1;
    }

    return 0;
}
