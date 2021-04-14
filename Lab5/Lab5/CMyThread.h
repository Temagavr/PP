#pragma once
#include "IThread.h"
#include <windows.h>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>


class CMyThread : public IThread
{
public:

	CMyThread() {};
	
	void AddThread() override;

	int RunAllThreads();

private:

	std::vector<HANDLE> handles;
	int value = 1;
	int* num = &value;
};
