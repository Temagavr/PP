#pragma once

class IThread
{
public:
	virtual void AddThread() = 0;
	virtual int RunAllThreads() = 0;
};
