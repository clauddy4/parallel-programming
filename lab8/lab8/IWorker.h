#pragma once

class ITask;

class IWorker
{
public:
	virtual bool ExecuteTask(ITask* task) = 0;
	virtual bool IsBusy() = 0;
	virtual ~IWorker() {}
};