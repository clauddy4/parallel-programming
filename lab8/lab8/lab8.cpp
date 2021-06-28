#include "Bitmap.h"
#include "BlurTask.h"
#include <Windows.h>
#include <iostream>
#include <vector>
#include "ThreadsPool.h"

enum class ExecuteMode
{
	CreateNewThread,
	UseThreadsPool
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	struct Params* params = (struct Params*)lpParam;
	BlurTask task(2, params);
	task.Execute();
	ExitThread(0);
}

void RunWithCreatingNewThreads(std::vector<Params> blurParams)
{
	// создание потоков
	HANDLE* handles = new HANDLE[blurParams.size()];
	for (size_t i = 0; i < blurParams.size(); i++)
	{
		handles[i] = CreateThread(NULL, i, &ThreadProc, &blurParams[i], CREATE_SUSPENDED, NULL);
	}

	// запуск потоков
	for (size_t i = 0; i < blurParams.size(); i++)
	{
		ResumeThread(handles[i]);
	}

	// ожидание окончания работы потоков
	WaitForMultipleObjects(blurParams.size(), handles, true, INFINITE);
}

void RunWithUsingThreadsPool(std::vector<Params> blurParams, int threadsPoolCount)
{
	ThreadsPool pool(threadsPoolCount);
	pool.Blur(blurParams);
}

std::vector<Params> GetBlurParams(Bitmap* in, int blocksCount)
{
	int partHeight = in->GetHeight() / blocksCount;
	int heightRemaining = in->GetHeight() % blocksCount;

	std::vector<Params> arrayParams;

	for (int i = 0; i < blocksCount; i++)
	{
		Params params;
		params.in = in;
		params.startWidth = 0;
		params.endWidth = in->GetWidth();
		params.startHeight = partHeight * i;
		params.endHeight = partHeight * (i + 1) + ((i == blocksCount - 1) ? heightRemaining : 0);

		arrayParams.push_back(params);
	}
	return arrayParams;
}

ExecuteMode StringToExecuteMode(std::string const& str)
{
	if (str == "newThread")
	{
		return ExecuteMode::CreateNewThread;
	}
	if (str == "pool")
	{
		return ExecuteMode::UseThreadsPool;
	}
	throw std::runtime_error("Cannot convert string \"" + str + "\" to execute mode");
}

void RunThreads(std::vector<Params> const& blurParams, ExecuteMode executeMode, int threadsPoolCount)
{
	switch (executeMode)
	{
	case ExecuteMode::CreateNewThread:
		RunWithCreatingNewThreads(blurParams);
		break;
	case ExecuteMode::UseThreadsPool:
		RunWithUsingThreadsPool(blurParams, threadsPoolCount);
		break;
	default:
		break;
	}
}

int main(int argc, const char** argv)
{
	unsigned int start = clock();

	if (argc != 5 && argc != 6)
	{
		std::cout << "Usage:" << std::endl;
		std::cout << "1 argument - newThread or pool" << std::endl;
		std::cout << "2 argument - number of blocks" << std::endl;
		std::cout << "3 argument - in directory" << std::endl;
		std::cout << "4 argument - out directory" << std::endl;
		std::cout << "5 argument - number of threads in pool" << std::endl;
		std::cout << "Example: pool 4 c:\\in c:\\out 2" << std::endl;

		return 1;
	}

	Bitmap bmp{ argv[3] };

	auto blurParams = GetBlurParams(&bmp, atoi(argv[2]));
	RunThreads(blurParams, StringToExecuteMode(argv[1]), argc == 6 ? atoi(argv[5]) : 0);

	bmp.Save(argv[4]);

	unsigned int end = clock();
	unsigned int duration = end - start;

	std::cout << duration << std::endl;

	return 0;
}
