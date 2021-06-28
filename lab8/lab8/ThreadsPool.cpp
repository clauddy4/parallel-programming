#include "ThreadsPool.h"
#include "Worker.h"

ThreadsPool::ThreadsPool(int threadsCount)
{
	for (int i = 0; i < threadsCount; i++)
	{
		m_threads.push_back(std::make_shared<Worker>());
	}
}

void ThreadsPool::Blur(std::vector<Params> blurParams)
{
	m_blurParams = blurParams;

	for (auto i = 0; i < blurParams.size(); ++i)
	{
		m_tasks.push_back(BlurTask(2, &m_blurParams[i]));
	}

	for (auto i = 0; i < blurParams.size(); ++i)
	{
		bool notExecute = true;
		while (notExecute)
		{
			for (auto thread : m_threads)
			{
				if (thread->ExecuteTask(&m_tasks[i]))
				{
					notExecute = false;
					break;
				}
			}
		}
	}
}

ThreadsPool::~ThreadsPool()
{
	for (auto thread : m_threads)
	{
		while (thread->IsBusy())
		{
		}
	}
}

