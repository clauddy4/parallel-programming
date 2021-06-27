#include "LogFileWriter.h"

LogFileWriter::LogFileWriter(std::string path)
	: m_output{ path }
{
}

void LogFileWriter::Write(List* data)
{
	while (data->GetCount() > 0)
	{
		m_output << data->Pop() << std::endl;
	}
	m_output.flush();
}