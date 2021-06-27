#pragma once
#include "ITask.h"

class Task : public ITask
{
public:
	Task(int number);
	void Execute() override;

private:
	int m_number;
};

