#pragma once
#include <cstdint>

typedef enum
{
    s0pos,  s0neg,
    s1pos,  s1neg,
    s2pos,  s2neg,
    s3pos,  s3neg,
    s4pos,  s4neg,
    s5pos,  s5neg,
    s6pos,  s6neg,
    s7pos,  s7neg,
    s8pos,  s8neg,
    s9pos,  s9neg,
    s10pos, s10neg,
    s11pos, s11neg,
    s12pos, s12neg,
    s13pos, s13neg,
    s14pos, s14neg,
    s15pos, s15neg,
    s16pos, s16neg,
    s17pos, s17neg
}
MAZE_SECTION;

// Approach direction
typedef enum
{
	adForward,
	adBackward
}
APPR_DIR;

// Exit direction
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
    APPR_DIR apprDir; // Approach direction
    EXIT_DIR exitDir; // Exit direction
    bool     arrived; // Target reached
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
    virtual void SetCurrentSection(MAZE_SECTION section) = 0;

    // Get the section where the robot is currently standing.
    virtual MAZE_SECTION GetCurrentSection() = 0;
};
