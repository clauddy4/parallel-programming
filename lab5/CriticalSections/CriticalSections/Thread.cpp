#include "Thread.h"
#include <windows.h>
#include <string>
#include <iostream>

using namespace std;

Thread::Thread(int* workingVariable)
    : m_workingVariable(workingVariable)
{
}

void Thread::Run()
{
    int value = *m_workingVariable;
    srand(time(NULL));
    int delta = rand() % 10 + 1;
    value += delta;
    *m_workingVariable = value;

    cout << "WorkingVarible = " + to_string(*m_workingVariable) + " | "
        + "Value = " + to_string(value) + "\n";

    if (*m_workingVariable != value)
    {
        cout << "Error! WorkingVariable is not equal to value." << endl;;
    }
}