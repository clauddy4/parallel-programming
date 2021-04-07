#include <windows.h>
#include <string>
#include <iostream>
#include <string>

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    std::string outStr = "Stream number " + std::to_string(int(lpParam)) + " doing his job\n";
    std::cout << outStr;
    ExitThread(0); // функция устанавливает код завершения потока в 0
}

int main(int argc, char* argv[])
{
    auto threadCount = atoi(argv[1]);

    // создание  потоков
    HANDLE* handles = new HANDLE[threadCount];
    for (size_t i = 0; i < threadCount; ++i)
    {
        handles[i] = CreateThread(NULL, 0, &ThreadProc, LPVOID(i), CREATE_SUSPENDED, NULL);
    }

    // запуск потоков
    for (size_t i = 0; i < threadCount; ++i)
    {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы потоков
    WaitForMultipleObjects(threadCount, handles, true, INFINITE);
    return 0;
}