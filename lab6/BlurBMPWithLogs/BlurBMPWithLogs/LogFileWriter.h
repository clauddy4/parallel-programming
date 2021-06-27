#pragma once

#include <cstdlib>
#include <string>
#include <fstream>
#include "List.h"

class LogFileWriter
{
public:
    LogFileWriter(std::string path = "output.txt");
    void Write(List* data);

private:
    std::ofstream m_output;
};

