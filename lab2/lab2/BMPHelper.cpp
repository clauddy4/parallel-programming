#include "BMPHelper.h"
#include "BMPFILE.h"

BMPFILE BMPHelper::ReadFromFile(std::string path)
{
	std::ifstream fin(path, std::ios::in | std::ios::binary);
	BMPFILE::BMPINFO bmpInfo;

	if (!fin)
	{
		std::cout << "Can't open file" << std::endl;
		return BMPFILE();
	}
	fin.read(reinterpret_cast<char*>(&bmpInfo.bmh), sizeof(bmpInfo.bmh));
	fin.read(reinterpret_cast<char*>(&bmpInfo.bi), sizeof(bmpInfo.bi));
	if (bmpInfo.bmh.bfType != 0x4d42)
	{
		std::cout << "Invalid type" << std::endl;
		return BMPFILE();
	}

	fin.seekg(bmpInfo.bmh.bfOffBits, std::ios::beg);
	std::vector<std::vector<BMPFILE::rgb>> pixels(bmpInfo.bi.biWidth, std::vector<BMPFILE::rgb>(bmpInfo.bi.biWidth));

	for (unsigned int i = 0; i < bmpInfo.bi.biHeight; i++) {
		for (unsigned int j = 0; j < bmpInfo.bi.biWidth; j++) {
			fin.read(reinterpret_cast<char*>(&pixels[i][j].r), sizeof(uint8_t));
			fin.read(reinterpret_cast<char*>(&pixels[i][j].g), sizeof(uint8_t));
			fin.read(reinterpret_cast<char*>(&pixels[i][j].b), sizeof(uint8_t));
			if (bmpInfo.bi.biBitCount == 32)
			{
				fin.read(reinterpret_cast<char*>(&pixels[i][j].a), sizeof(uint8_t));
			}
		}
	}

	return BMPFILE(bmpInfo, pixels);
}


void BMPHelper::Blur(BMPFILE* originalBmp, BMPFILE* bluredBmp, int startHeight, int endHeight, std::ofstream* fout, clock_t startTime, int threadNumber, int radius)
{
	float rs = std::ceil(radius * 2.57);
	for (int i = 0; i < originalBmp->GetWidth() - 1; ++i)
	{
		for (int j = startHeight; j <= endHeight; ++j)
		{
			double r = 0, g = 0, b = 0;
			double wsum = 0;

			for (int iy = i - rs; iy < i + rs + 1; ++iy)
			{
				for (int ix = j - rs; ix < j + rs + 1; ++ix)
				{
					auto x = std::min(static_cast<int>(endHeight) - 1, std::max(0, ix));
					auto y = std::min(static_cast<int>(originalBmp->GetWidth()) - 1, std::max(0, iy));
					auto dsq = ((ix - j) * (ix - j)) + ((iy - i) * (iy - i));
					auto wght = std::exp(-dsq / (2.0 * radius * radius)) / (M_PI * 2.0 * radius * radius);

					BMPFILE::rgb pixel = originalBmp->GetPixel(x, y);
					r += pixel.r * wght;
					g += pixel.g * wght;
					b += pixel.b * wght;
					wsum += wght;
				}
			}

			BMPFILE::rgb pixel = originalBmp->GetPixel(j, i);
			BMPFILE::rgb bluredPixel = BMPFILE::rgb();
			bluredPixel.r = std::round(r / wsum);
			bluredPixel.g = std::round(g / wsum);
			bluredPixel.b = std::round(b / wsum);
			bluredBmp->SetPixel(j, i, bluredPixel);

			//*fout << clock() - startTime << std::endl;
		}
	}
	*fout << clock() - startTime << std::endl;
}


void BMPHelper::WriteBMPFile(BMPFILE* bmp, std::string path)
{
	std::ofstream fout(path, std::ios::out | std::ios::binary);

	BMPFILE::BMPINFO tmp = bmp->GetInfo();

	fout.write(reinterpret_cast<char*>(&tmp), sizeof(BMPFILE::BMPINFO));
	fout.seekp(bmp->GetInfo().bmh.bfOffBits, std::ios::beg);

	std::vector<std::vector<BMPFILE::rgb>> pixels = bmp->GetPixels();

	for (std::vector<BMPFILE::rgb> pixelLine : pixels)
	{
		for (BMPFILE::rgb pixel : pixelLine)
		{
			fout.write(reinterpret_cast<char*>(&pixel.r), sizeof(uint8_t));
			fout.write(reinterpret_cast<char*>(&pixel.g), sizeof(uint8_t));
			fout.write(reinterpret_cast<char*>(&pixel.b), sizeof(uint8_t));
			if (bmp->GetInfo().bi.biBitCount == 32)
			{
				fout.write(reinterpret_cast<char*>(&pixel.a), sizeof(uint8_t));
			}
		}
	}
}