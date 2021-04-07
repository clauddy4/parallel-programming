#include "BMPFILE.h"
#include <algorithm>
#include <time.h>	 
#include <fstream>

constexpr auto M_PI = 3.141592653589793238462643383279502884L;

static class BMPHelper
{
public:
	static BMPFILE ReadFromFile(std::string path);
	static void BlurByWidth(BMPFILE* originalBmp, BMPFILE* bluredBmp, int startWidth, int endWidth, std::ofstream* fout, clock_t startTime, int threadNumber, int radius = 5);
	static void WriteBMPFile(BMPFILE* bmp, std::string path);

	BMPHelper() {};
	~BMPHelper() {};
};