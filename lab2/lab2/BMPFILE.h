#pragma once
#include <vector>
#include <fstream>
#include <iostream>

class BMPFILE
{
public:
#pragma pack(2)
	typedef struct
	{
		uint16_t bfType;
		uint32_t bfSize;
		uint16_t bfReserved1;
		uint16_t bfReserved2;
		uint32_t bfOffBits;
	} BITMAPFILEHEADER;
#pragma()

#pragma pack(2)
	struct BITMAPINFOHEADER
	{
		uint32_t biSize;
		uint32_t biWidth;
		uint32_t biHeight;
		uint16_t biPlanes;
		uint16_t biBitCount;
		uint32_t biCompression;
		uint32_t biSizeImage;
		uint32_t biXPelsPerMeter;
		uint32_t biYPelsPerMeter;
		uint32_t biClrUsed;
		uint32_t biClrImportant;
	};
#pragma()

	struct BMPINFO
	{
		BITMAPFILEHEADER bmh;
		BITMAPINFOHEADER bi;
	};

	struct rgb
	{
		uint8_t r, g, b, a;
	};

	BMPFILE(BMPINFO header, std::vector<std::vector<rgb>> pixels);
	BMPFILE() {};
	~BMPFILE() {};

	BMPINFO GetInfo();
	std::vector<std::vector<rgb>> GetPixels();
	uint32_t GetWidth();
	uint32_t GetHeight();
	rgb GetPixel(int x, int y);
	void SetPixel(int x, int y, rgb pixel);
private:

	BMPINFO _bmpInfo;
	std::vector<std::vector<rgb>> _pixels;
};