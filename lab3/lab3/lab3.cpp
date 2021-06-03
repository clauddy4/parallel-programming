#include <windows.h>
#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

DWORD startTime = 0;
SYSTEMTIME sttime;

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
    int param = (int)lpParam;
    for (int j = 0; j < 10; j++)
    {
        for (int i = 0; i < 999999; i++) {
            int doNothing = log(555);
        }
        SYSTEMTIME curtime;
        GetSystemTime(&curtime);
        DWORD currentTime = GetTickCount();

        int calcul1 = sttime.wSecond * 1000 + sttime.wMilliseconds;
        int calcul2 = curtime.wSecond * 1000 + curtime.wMilliseconds;
        string out = to_string(param) + "\n" + to_string(calcul2 - calcul1) + "\n";
        cout << out;
    }

    ExitThread(0); // функция устанавливает код завершения потока в 0
}


int main()
{
    HANDLE hProcess = GetCurrentProcess();
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    unsigned int nMaxProcessorMask = (1 << 8) - 1;
    SetProcessAffinityMask(hProcess, nMaxProcessorMask);

    system("pause");

    startTime = GetTickCount();
    GetSystemTime(&sttime);
    int count_threads = 2;
    HANDLE* handles = new HANDLE[count_threads];
    for (int i = 0; i < count_threads; i++) {
        int ThreadCount = i + 1;
        handles[i] = CreateThread(NULL, 0, &ThreadProc, (LPVOID)ThreadCount, CREATE_SUSPENDED, NULL);
    }
    // запуск двух потоков
    for (int i = 0; i < count_threads; i++) {
        ResumeThread(handles[i]);
    }

    // ожидание окончания работы двух потоков
    WaitForMultipleObjects(count_threads, handles, true, INFINITE);
    return 0;
}