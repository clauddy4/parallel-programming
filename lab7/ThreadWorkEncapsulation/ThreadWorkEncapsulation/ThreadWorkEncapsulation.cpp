#include <iostream>
#include <string>
#include <iostream>
#include "Task.h"
#include "Worker.h"

int main(int argc, char* argv[])
{
    auto threadCount = 4;

	IWorker* worker = new Worker();

	for (int i = 0; i < threadCount; i++)
	{
		ITask* task = new Task(i);
		Sleep(2000);
		std::cout << worker->ExecuteTask(task) << std::endl;
	}

	delete worker; 
	return 0;
}
