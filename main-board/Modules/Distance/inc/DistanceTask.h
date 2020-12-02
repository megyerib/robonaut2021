#pragma once

#include "CyclicTask.h"
#include "Distance.h"

class DistanceTask : public CyclicTask
{
    Distance* distance = nullptr;

    DistanceTask();

    virtual void TaskInit() override;
    virtual void TaskFunction() override;

public:
    static DistanceTask* Init();
};
