#include "BlurTask.h"
#include <cmath>

BlurTask::BlurTask(int radius, Params* params)
	: radius(radius)
	, params(params)
{
}

void BlurTask::Execute()
{
	auto rs = ceil(radius * 2.57);
	for (auto i = params->startHeight; i < params->endHeight; ++i)
	{
		for (auto j = params->startWidth; j < params->endWidth; ++j)
		{
			double r = 0, g = 0, b = 0;
			double count = 0;

			for (auto iy = i - rs; iy < i + rs + 1; ++iy)
			{
				for (auto ix = j - rs; ix < j + rs + 1; ++ix)
				{
					auto x = min(static_cast<int>(params->endWidth) - 1, max(0, ix));
					auto y = min(static_cast<int>(params->endHeight) - 1, max(0, iy));

					long long dsq = ((ix - j) * (ix - j)) + ((iy - i) * (iy - i));
					double wght = std::exp(-dsq / (2.0 * radius * radius)) / (3.14 * 2.0 * radius * radius);

					rgb32* pixel = params->in->GetPixel(x, y);

					r += pixel->r * wght;
					g += pixel->g * wght;
					b += pixel->b * wght;
					count += wght;
				}
			}

			rgb32* pixel = params->in->GetPixel(j, i);
			pixel->r = std::round(r / count);
			pixel->g = std::round(g / count);
			pixel->b = std::round(b / count);
		}
	}
}
