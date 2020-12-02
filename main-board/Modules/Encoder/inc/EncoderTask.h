#pragma once

#include "CyclicTask.h"

class EncoderTask : public CyclicTask
{
public:
    static EncoderTask& Init();

private:
	EncoderTask();

    virtual void TaskInit() override;
    virtual void TaskFunction() override;
};
