#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>
#include "Bitmap.h"

struct BitmapParameters
{
	Bitmap* bitmap;
	uint32_t endWidth;
	uint32_t startHeight;
	uint32_t endHeight;
};

void Blur(BitmapParameters* parameters, int radius)
{
	float rs = ceil(radius * 2.57);
	for (auto i = parameters->startHeight; i < parameters->endHeight; ++i)
	{
		for (auto j = 0; j < parameters->endWidth; ++j)
		{
			double r = 0, g = 0, b = 0;
			double count = 0;

			for (int iy = i - rs; iy < i + rs + 1; ++iy)
			{
				for (int ix = j - rs; ix < j + rs + 1; ++ix)
				{
					auto x = min(static_cast<int>(parameters->endWidth) - 1, max(0, ix));
					auto y = min(static_cast<int>(parameters->endHeight) - 1, max(0, iy));

					long long dsq = ((ix - j) * (ix - j)) + ((iy - i) * (iy - i));
					double wght = std::exp(-dsq / (2.0 * radius * radius)) / (3.14 * 2.0 * radius * radius);

					rgb32* pixel = parameters->bitmap->GetPixel(x, y);

					r += pixel->r * wght;
					g += pixel->g * wght;
					b += pixel->b * wght;
					count += wght;
				}
			}

			rgb32* pixel = parameters->bitmap->GetPixel(j, i);
			pixel->r = std::round(r / count);
			pixel->g = std::round(g / count);
			pixel->b = std::round(b / count);
		}
	}
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam)
{
	struct BitmapParameters* parameters = (struct BitmapParameters*)lpParam;
	Blur(parameters, 2);
	ExitThread(0); // функция устанавливает код завершения потока в 0
}

void ThreadsRunner(Bitmap* bitmap, int threadsCount, int coreCount)
{
	int stripe = bitmap->GetHeight() / threadsCount;
	int remainder = bitmap->GetHeight() % threadsCount;

	BitmapParameters* arrayParams = new BitmapParameters[threadsCount];
	for (int i = 0; i < threadsCount; i++)
	{
		BitmapParameters parameters;
		parameters.bitmap = bitmap;
		parameters.endWidth = bitmap->GetWidth();
		parameters.startHeight = stripe * i;
		parameters.endHeight = stripe * (i + 1) + ((i == threadsCount - 1) ? remainder : 0);
		arrayParams[i] = parameters;
	}

	// создание потоков
	HANDLE* handles = new HANDLE[threadsCount];
	for (int i = 0; i < threadsCount; i++)
	{
		handles[i] = CreateThread(NULL, i, &ThreadProc, &arrayParams[i], CREATE_SUSPENDED, NULL);
		SetThreadAffinityMask(handles[i], (1 << coreCount) - 1);
	}

	// запуск потоков
	for (int i = 0; i < threadsCount; i++)
	{
		ResumeThread(handles[i]);
	}

	// ожидание окончания работы потоков
	WaitForMultipleObjects(threadsCount, handles, true, INFINITE);

	delete[] arrayParams;
	delete[] handles;
}

int main(int argc, const char** argv)
{
	unsigned int start = clock();

	if (argc != 5)
	{
		std::cout << "Usage: BlurBMP.exe <image to blur .bmp> <blurred image .bmp> <thead count> <core count>" << std::endl;
		return 1;
	}

	Bitmap bmp{ argv[1] };
	ThreadsRunner(&bmp, atoi(argv[3]), atoi(argv[4]));
	bmp.Save(argv[2]);

	unsigned int end = clock();
	unsigned int duration = end - start;
	std::cout << duration << std::endl;

	return 0;
}