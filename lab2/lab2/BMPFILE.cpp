#include "BMPFILE.h"

BMPFILE::BMPFILE(BMPINFO header, std::vector<std::vector<rgb>> pixels)
	:_bmpInfo(header)
	, _pixels(pixels)
{
}

BMPFILE::BMPINFO BMPFILE::GetInfo()
{
	return _bmpInfo;
}

std::vector<std::vector<BMPFILE::rgb>> BMPFILE::GetPixels()
{
	return _pixels;
}


uint32_t BMPFILE::GetWidth()
{
	return _bmpInfo.bi.biWidth;
}

uint32_t BMPFILE::GetHeight()
{
	return _bmpInfo.bi.biHeight;
}

BMPFILE::rgb BMPFILE::GetPixel(int x, int y)
{
	return _pixels[y][x];
}

void BMPFILE::SetPixel(int x, int y, rgb pixel)
{
	_pixels[y][x] = pixel;
}