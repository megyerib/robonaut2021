#pragma once
#include <cstdint>

typedef enum : uint8_t
{
    sApos,  sAneg,
    sBpos,  sBneg,
    sCpos,  sCneg,
    sDpos,  sDneg,
    sEpos,  sEneg,
    sFpos,  sFneg,
    sGpos,  sGneg,
    sHpos,  sHneg,
    sIpos,  sIneg,
    sJpos,  sJneg,
    sKpos,  sKneg,
    sLpos,  sLneg,
    sMpos,  sMneg,
    sNpos,  sNneg,
    sOpos,  sOneg,
    sPpos,  sPneg,
    sQpos,  sQneg,
    sRpos,  sRneg,
    sSpos,  sSneg,
    sTpos,  sTneg,
    sUpos,  sUneg,
    sVpos,  sVneg,
    sStpos, sStneg,
    sExpos, sExneg
}
MAZE_SECTION;

// Approach direction
typedef enum : uint8_t
{
	adForward,
	adBackward
}
APPR_DIR;

// Exit direction
typedef enum : uint8_t
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
