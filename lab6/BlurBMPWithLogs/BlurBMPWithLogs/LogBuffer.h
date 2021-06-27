#pragma once
#include <Windows.h>
#include "LogFileWriter.h"
#include "List.h"

class LogBuffer
{
public:
    LogBuffer();
    ~LogBuffer();
    void Log(int value);

private:
    constexpr static int MAX_BUFFER_COUNT = 256;
    LogFileWriter m_fileWriter;

    CRITICAL_SECTION m_criticalSection;
    List m_list;

    HANDLE m_threadHandle;
    HANDLE m_event;

    static DWORD WINAPI LogSizeMonitoring(CONST LPVOID lp_param);
};

