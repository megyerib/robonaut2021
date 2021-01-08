#include "MazeNav_AlwaysLeft.h"

MazeNav_AlwaysLeft::MazeNav_AlwaysLeft()
{

}

MAZE_MOVE MazeNav_AlwaysLeft::GetNextMove(uint8_t target)
{
    (void) target;

    MAZE_MOVE ret =
    {
        .apprDir = adForward,
        .exitDir = edFrontLeft,
        .arrived = false
    };

    return ret;
}

void MazeNav_AlwaysLeft::SetSection(MAZE_SECTION section)
{
    (void) section;
}