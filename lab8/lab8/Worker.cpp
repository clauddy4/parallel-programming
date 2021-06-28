#include "Worker.h"
#include "ITask.h"

DWORD WINAPI Worker::ThreadProc(CONST LPVOID lpParam)
{
	auto task = (ITask*)lpParam;
	task->Execute();
	ExitThread(0);
}

bool Worker::ExecuteTask(ITask* task)
{
	if (IsBusy())
	{
		return false;
	}
	if (m_thread = CreateThread(NULL, 0, &ThreadProc, task, CREATE_SUSPENDED, NULL))
	{
		ResumeThread(m_thread);
		return true;
	}

	return false;
}

bool Worker::IsBusy()
{
	DWORD exitCode;
	GetExitCodeThread(m_thread, &exitCode);
	return exitCode	== STILL_ACTIVE;
}

Worker::~Worker()
{
	WaitForSingleObject(m_thread, INFINITE);
}


