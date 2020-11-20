#pragma once

#include "CyclicTask.h"
#include "Encoder.h"

class EncoderTask : public CyclicTask
{
public:
	static EncoderTask* Init();

private:
	Encoder* encoder = nullptr;

	EncoderTask();

	void TaskInit() override;
	void TaskFunction() override;
};
