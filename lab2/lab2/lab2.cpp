#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>
#include <time.h>
#include "BMPFILE.h"
#include "BMPHelper.h"

struct Params
{
	clock_t startTime;
	int threadNumber;
	std::ofstream* fout;
	BMPFILE* bmp;
	BMPFILE* blured;
	int start;
	int end;
};

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	struct Params* params = (struct Params*)lpParam;
	BMPHelper::Blur(&*params->bmp, &*params->blured, params->start, params->end, &*params->fout, params->startTime, params->threadNumber);
	ExitThread(0);
}

int main(int argc, char* argv[])
{
	clock_t start = clock();

	std::string bmpFileName = "../in.bmp";
	std::string bluredFileName = "../out.bmp";

	std::vector<std::ofstream> outFiles;

	BMPFILE bmp = BMPHelper::ReadFromFile(bmpFileName);

	BMPFILE blured = BMPFILE(bmp);

	int threadsCount = 16;
	int coreCount = 1;
	uint32_t height = (bmp.GetHeight() - 1) / threadsCount;
	std::vector<int> priority;
	for (int i = 0; i < threadsCount; i++)
	{
		outFiles.push_back(std::ofstream("output" + std::to_string(i) + ".txt"));
	}

	std::vector<Params> paramsToThread;

	int minHeight = 0;

	for (int i = 0; i < threadsCount; i++)
	{
		Params params;
		params.startTime = start;
		params.threadNumber = i;
		params.fout = &outFiles[i];
		params.bmp = &bmp;
		params.blured = &blured;
		params.start = minHeight;
		params.end = i == threadsCount - 1 ? bmp.GetHeight() - 1 : minHeight + height;
		paramsToThread.push_back(params);
		minHeight += height;
	}

	HANDLE* handles = new HANDLE[threadsCount];

	for (size_t i = 0; i < threadsCount; i++)
	{
		handles[i] = CreateThread(NULL, i, &ThreadProc, &paramsToThread[i], CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(handles[i], (1 << coreCount) - 1);
	}

	for (int i = 0; i < threadsCount; i++)
	{
		ResumeThread(handles[i]);
	}

	WaitForMultipleObjects(threadsCount, handles, true, INFINITE);

	BMPHelper::WriteBMPFile(&blured, bluredFileName);
}