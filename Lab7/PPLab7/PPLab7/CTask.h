#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include "ITask.h"

class CTask : public ITask
{
public:
	CTask(int value) :number(value)
	{}

	void Execute() override
	{
		std::stringstream temp;
		temp << "Поток №" << number << " выполняется" << std::endl;
		std::cout << temp.str();
	}

private:
	int number;
};