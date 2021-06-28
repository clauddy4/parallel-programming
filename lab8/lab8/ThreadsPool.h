#pragma once
#include "BlurTask.h"
#include "IWorker.h"
#include <Windows.h>
#include <memory>
#include <vector>

class ThreadsPool
{
public:
	ThreadsPool(int threadsCount);
	void Blur(std::vector<Params> m_blurParams);
	~ThreadsPool();

private:
	std::vector<std::shared_ptr<IWorker>> m_threads;
	std::vector<Params> m_blurParams;
	std::vector<BlurTask> m_tasks;
};
