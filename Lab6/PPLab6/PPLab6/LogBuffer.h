#pragma once
#include "LogFileWriter.h"
#include "LinkedList.h"
#include <windows.h>

class LogBuffer
{
public:
	LogBuffer(LogFileWriter* writter)
		: writer(writter)
		, criticaSection(CRITICAL_SECTION())
	{
		if (!InitializeCriticalSectionAndSpinCount(&criticaSection, 0x00000400))
		{
			throw std::exception("Faield to create critical section");
		}

		writeEvent = CreateEvent(nullptr, TRUE, FALSE, TEXT("WriteEvent"));

		InitLogSizeMonitoringThread();
	}

	~LogBuffer()
	{
		CloseHandle(logSizeMonitoringThread);
		CloseHandle(writeEvent);
		DeleteCriticalSection(&criticaSection);
	}

	void Log(std::string str)
	{
		EnterCriticalSection(&criticaSection);

		if (list.GetSize() >= 500)
		{
			ReleaseBuffer();
		}

		list.Append(str);

		LeaveCriticalSection(&criticaSection);
	}

	void ReleaseBuffer()
	{
		if (!SetEvent(writeEvent))
		{
			throw std::runtime_error("Failed to set event");
		}

		if (WaitForSingleObject(logSizeMonitoringThread, INFINITE) == WAIT_OBJECT_0) {
			ResetEvent(writeEvent);
			InitLogSizeMonitoringThread();
		}
		else
		{
			throw std::runtime_error("Failed to set event");
		}
	}

	void InitLogSizeMonitoringThread()
	{
		logSizeMonitoringThread = CreateThread(nullptr, 0, &LogSizeMonitoringThread, static_cast<void*>(this), 0, nullptr);
	}

private:

	static DWORD WINAPI LogSizeMonitoringThread(CONST LPVOID logBufferLpVoid)
	{
		LogBuffer* logBuffer = static_cast<LogBuffer*>(logBufferLpVoid);

		if (WaitForSingleObject(logBuffer->writeEvent, INFINITE) == WAIT_OBJECT_0)
		{
			for (const auto& el : logBuffer->list)
			{
				logBuffer->writer->Write(el);
			}

			logBuffer->list.Clear();
		}

		ExitThread(EXIT_SUCCESS);
	}

	CLinkedList<std::string> list;

	CRITICAL_SECTION criticaSection;

	HANDLE logSizeMonitoringThread;

	HANDLE writeEvent;

	LogFileWriter* writer;
};

