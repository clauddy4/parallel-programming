#include <windows.h>
#include <string>
#include <iostream>
#include "Thread.h"

using namespace std;

const int THREAD_COUNT = 2;

CRITICAL_SECTION CriticalSection;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    Thread* thread = static_cast<Thread*>(lpParam);

    thread->Run();

    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{
    char ch;
    cin >> ch;

    int workingVariable = 1;

    // создание  потоков
    HANDLE* handles = new HANDLE[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++)
    {
        Thread thread = Thread(&workingVariable);
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &thread, CREATE_SUSPENDED, NULL);
    }

    // запуск потоков
    for (size_t i = 0; i < THREAD_COUNT; i++)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы потоков
    WaitForMultipleObjects(THREAD_COUNT, handles, true, INFINITE);

    return 0;
}