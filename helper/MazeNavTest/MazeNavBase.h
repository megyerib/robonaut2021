#pragma once
#include <cstdint>

typedef enum
{
	s0pos, s1pos, s2pos,

	s0neg, s1neg, s2neg
}
MAZE_SECTION;

typedef enum
{
	adForward,
	adBackward
}
APPR_DIR;

typedef enum
{
	edFrontLeft,
	edFrontMid,
	edFrontRight,
	edRearLeft,
	edRearMid,
	edRearRight
}
EXIT_DIR;

typedef struct
{
	APPR_DIR apprDir;
	EXIT_DIR exitDir;
    bool     arrived;
}
MAZE_MOVE;

class MazeNavBase
{
public:
	// Get the next move towards the target section (no direction specified) AND
	// set the current section accordingly (like the move was already executed).
	// Current section will be set without any external command.
    virtual MAZE_MOVE GetNextMove(uint8_t target) = 0;

	// Set the section where the robot is currently standing.
	// Needs to be called only at start or during testing.
    virtual void SetSection(MAZE_SECTION section) = 0;
};
