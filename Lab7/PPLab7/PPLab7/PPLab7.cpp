#include <iostream>
#include "CTask.h"
#include "CWorker.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    CWorker worker;
    const int number = 10;
    ITask* my_class[number];

    for (int i = 0; i < number; i++)
    {
        my_class[i] = new CTask(i);
        Sleep(0.5);
        worker.ExecuteTask(my_class[i]);
    }

    return 0;
}
