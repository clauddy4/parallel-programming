#pragma once
#include "IWorker.h"
#include <Windows.h>

class Worker : public IWorker
{
public:
	bool ExecuteTask(ITask* task) override;
	bool IsBusy() override;
	~Worker();

private:
	static DWORD WINAPI ThreadProc(CONST LPVOID lpParam);
	
	HANDLE m_thread;
};
