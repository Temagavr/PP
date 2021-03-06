#include "Blur.h"
#include "bitmap_image.hpp"
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <ctime>

const int BLUR_RADIUS = 20;

struct ThreadParams
{
	int startBlurPixelX = 0;
	int startBlurPixelY = 0;

	int endBlurPixelX = 0;
	int endBlurPixelY = 0;

	bitmap_image input;

	bitmap_image* output = nullptr;

	int blurRadius = 0;
};

std::ostream& operator <<(std::ostream& out, const ThreadParams& c)
{
	out << "Start x: " << c.startBlurPixelX << ", end x: " << c.endBlurPixelX << ", start y: " << c.startBlurPixelY <<
		", end y: " << c.endBlurPixelY;
	return out;
}

DWORD WINAPI AsyncBlurFunc(CONST LPVOID lpParam)
{
	auto settings = static_cast<ThreadParams*>(lpParam);

	for (int height = settings->startBlurPixelY; height < settings->endBlurPixelY; ++height)
	{
		for (int width = settings->startBlurPixelX; width < settings->endBlurPixelX; ++width)
		{
			std::vector<rgb_t> pixels;

			for (int i = height - BLUR_RADIUS; i <= height + BLUR_RADIUS; ++i)
			{
				for (int j = width - BLUR_RADIUS; j <= width + BLUR_RADIUS; ++j)
				{
					int minHeight = min(int(settings->input.height()) - 1, max(i, 0));

					int minWidth = min(int(settings->input.width()), max(j, 0));

					rgb_t pixel = settings->input.get_pixel(minWidth, minHeight);

					pixels.push_back(pixel);
				}
			}

			int totalRed = 0, totalGreen = 0, totalBlue = 0;

			for (const auto& pixel : pixels)
			{
				totalRed += pixel.red;
				totalGreen += pixel.green;
				totalBlue += pixel.blue;
			}

			rgb_t color;
			color.red = static_cast<uint8_t>(totalRed / pixels.size());
			color.green = static_cast<uint8_t>(totalGreen / pixels.size());
			color.blue = static_cast<uint8_t>(totalBlue / pixels.size());

			settings->output->set_pixel(width, height, color);
		}
	}

	ExitThread(0);
}

void BlurBmp(const BlurSettings& settings)
{
	std::vector<HANDLE> threadHandles;

	std::vector<ThreadParams*> settingsPtr;

	bitmap_image inputImage(settings.input);
	bitmap_image outputImage(inputImage);

	unsigned int w = inputImage.width();
	unsigned int h = inputImage.height();

	if (settings.threadsCount == 0)
	{
		throw std::exception("Thread count can not be 0");
	}

	std::cout << "Cores: " << settings.coresCount << " Threads: " << settings.threadsCount << std::endl;

	const unsigned int wPerThread = w / settings.threadsCount;

	auto affinityMask = (1 << int(settings.coresCount)) - 1;

	for (int i = 0; i < settings.threadsCount; ++i)
	{
		ThreadParams* threadSettings = new ThreadParams();
		threadSettings->blurRadius = BLUR_RADIUS;
		threadSettings->input = inputImage;
		threadSettings->output = &outputImage;
		threadSettings->startBlurPixelY = 0;
		threadSettings->endBlurPixelY = h;
		threadSettings->startBlurPixelX = (wPerThread * i);

		if (i + 1 != settings.threadsCount)
		{
			threadSettings->endBlurPixelX = wPerThread * (i + 1);
		}
		else
		{
			threadSettings->endBlurPixelX = w;
		}

		settingsPtr.push_back(threadSettings);

		threadHandles.push_back(
			CreateThread(nullptr, 0, &AsyncBlurFunc, threadSettings, CREATE_SUSPENDED, nullptr)
		);

		SetThreadAffinityMask(threadHandles[i], affinityMask);
	}


	std::clock_t start = std::clock();

	for (const auto& threadHandle : threadHandles)
	{
		ResumeThread(threadHandle);
	}

	WaitForMultipleObjects(static_cast<DWORD>(threadHandles.size()), threadHandles.data(), true, INFINITE);

	for (auto element : settingsPtr)
	{
		delete element;
	}

	outputImage.save_image(settings.output);
}