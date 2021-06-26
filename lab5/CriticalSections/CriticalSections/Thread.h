#pragma once

class Thread
{
public:
    Thread(int* workingVariable);

    void Run();

private:
    int* m_workingVariable;
};