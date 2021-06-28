#pragma once
#include "Bitmap.h"
#include "ITask.h"

struct Params
{
	Bitmap* in;
	uint32_t startHeight;
	uint32_t endHeight;
	uint32_t startWidth;
	uint32_t endWidth;
};

class BlurTask : public ITask
{
public:
	BlurTask(int radius, Params* params);
	void Execute() override;

private:
	int radius;
	Params* params;
};
