#pragma once

#include "MazeNavBase.h"

class MazeNav_AlwaysLeft : public MazeNavBase
{
public:
    MazeNav_AlwaysLeft();

    virtual MAZE_MOVE GetNextMove(uint8_t target) override;
    virtual void SetCurrentSection(MAZE_SECTION section) override;
    virtual MAZE_SECTION GetCurrentSection() override;
};
