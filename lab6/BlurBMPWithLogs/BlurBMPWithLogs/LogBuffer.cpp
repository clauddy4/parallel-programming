#include "LogBuffer.h"
#include <iostream>

constexpr int MAX_BUFFER_COUNT = 1000;

LogBuffer::LogBuffer()
{
	InitializeCriticalSectionAndSpinCount(&m_criticalSection, 1);
	m_event = CreateEvent(nullptr, TRUE, FALSE, TEXT("BufferIsOver"));
	m_threadHandle = CreateThread(NULL, 0, &LogSizeMonitoring, this, 0, NULL);
}

LogBuffer::~LogBuffer()
{
	DeleteCriticalSection(&m_criticalSection);
}

void LogBuffer::Log(int value)
{
	EnterCriticalSection(&m_criticalSection);
	if (m_list.GetCount() > MAX_BUFFER_COUNT)
	{
		SetEvent(m_event);
		DWORD wait = WaitForSingleObject(m_threadHandle, INFINITE);
		ResetEvent(m_event);
		m_threadHandle = CreateThread(NULL, 0, &LogSizeMonitoring, this, 0, NULL);
	}

	m_list.Append(value);

	LeaveCriticalSection(&m_criticalSection);
}

DWORD WINAPI LogBuffer::LogSizeMonitoring(CONST LPVOID lpParam)
{
	auto logBuffer = (LogBuffer*)lpParam;

	DWORD wait = WaitForSingleObject(logBuffer->m_event, INFINITE);

	if (wait == WAIT_OBJECT_0)
	{
		logBuffer->m_fileWriter.Write(&logBuffer->m_list);
	}

	ExitThread(0);
}