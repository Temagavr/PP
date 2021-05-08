#pragma once
#include <string>
#include <vector>
#include <ostream>

struct BlurSettings {
	BlurSettings(std::string inputName, std::string outputName, int coresCount, int threadsCount, std::vector<int> priority)
		: input(std::move(inputName))
		, output(std::move(outputName))
		, coresCount(coresCount)
		, threadsCount(threadsCount)
		, threadPriorities(std::move(priority))
	{
		if (int(threadPriorities.size()) != threadsCount)
		{
			throw std::exception("Thread priorities size should be equal to threads count");
		}
	}

	std::string input;
	std::string output;

	int coresCount;
	int threadsCount;

	std::vector<int> threadPriorities;
};

void BlurBmp(const BlurSettings& settings);