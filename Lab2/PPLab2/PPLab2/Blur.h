#pragma once
#include <string>
#include <ostream>

struct BlurSettings {
	BlurSettings(std::string inputName, std::string outputName, int coresCount, int threadsCount)
		: input(std::move(inputName))
		, output(std::move(outputName))
		, coresCount(coresCount)
		, threadsCount(threadsCount)
	{
	}

	std::string input;
	std::string output;

	int coresCount;
	int threadsCount;
};

void BlurBmp(const BlurSettings& settings);