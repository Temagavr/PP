#include "CPool.h"
#include <mutex>
#include <Windows.h>

struct CPool::SPoolImpl {
	unsigned threadCount = 0;

	std::vector<std::shared_ptr<ITask>> tasks;

	std::vector<HANDLE> handles;
};

static DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	ITask* task = (ITask*)lpParam;
	task->Execute();
	ExitThread(0);
}

CPool::CPool(std::vector<std::shared_ptr<ITask>> tasks, unsigned threadsCount)
{
	m_impl = new SPoolImpl();

	m_impl->tasks = std::move(tasks);

	m_impl->handles.resize(m_impl->tasks.size());

	for (size_t i = 0; i < m_impl->handles.size(); ++i)
	{
		m_impl->handles[i] = CreateThread(nullptr, 0, &ThreadProc, m_impl->tasks[i].get(), CREATE_SUSPENDED, nullptr);
	}

	m_impl->threadCount = std::move(threadsCount);
}

CPool::~CPool()
{
	delete m_impl;
}

void CPool::Execute()
{
	int count = 0;

	for (size_t i = 0; i < m_impl->handles.size(); ++i)
	{
		ResumeThread(m_impl->handles[i]);
		count++;

		if (m_impl->threadCount == count)
		{
			WaitForMultipleObjects((DWORD)i + 1, m_impl->handles.data(), true, INFINITE);
			count = 0;
		}
	}

	WaitForMultipleObjects((DWORD)m_impl->handles.size(), m_impl->handles.data(), true, INFINITE);
}
