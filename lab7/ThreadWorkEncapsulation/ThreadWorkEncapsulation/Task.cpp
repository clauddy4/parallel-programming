#include "Task.h"
#include <string>
#include <iostream>
#include <Windows.h>

Task::Task(int number)
	: m_number(number)
{
}

void Task::Execute()
{
	Sleep(1000);
	std::cout << "Thread number " + std::to_string(m_number) + " is doing his job\n";
}
