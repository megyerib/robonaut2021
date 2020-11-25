#pragma once

#include "Line.h"
#include "Receiver.h"

typedef enum
{
	None,
	Single,
	Acceleration,
	Braking,
    Exit,
    ExitReverse,
    Fork3,
    Fork2,
    Junction3,
    Junction2,
    DeadEnd,
    CrossingPoint,

	TrackTypeNum
}
TrackType;

const char trackTypeNames[TrackTypeNum][15] =
{
	"-None",
	"-Single",
	"-Acceleration",
	"-Braking",
	"-Exit",
	"-ExitReverse",
	"-Fork3",
	"-Fork2",
	"-Junction3",
	"-Junction2",
	"-DeadEnd",
	"-CrossingPoint"
};

typedef enum
{
	lt_1 = 0,
	lt_2Near,
	lt_2Far,
	lt_3Near,
	lt_3Far,
	lt_End,

	lt_Num
}
LineType;

const char lineTypeNames[lt_Num][8] =
{
	" 1",
	" 2 near",
	" 2 far",
	" 3 near",
	" 3 far",
	" end"
};

typedef enum
{
	td_1 = 0,
	td_2Near,
	td_2Far,
	td_3Near,
	td_3Far,
	td_2Junction,
	td_3Junction,
	td_2Fork,
	td_3Fork,
	td_End,

	td_Num
}
TrackDetectorState;

typedef enum
{
    ld_NoLine,
    ld_Left,
    ld_Middle,
    ld_Right
} LineDirection;

typedef struct
{
	Receiver*           receiver;        // Line sensor receiver

	LineInput           input;           // Structure from the line receiver
	float               pos;             // Latest position of the detected line
	uint32_t            chosenIndex;     // Line which we are currently following

	uint32_t            filteredCnt;     //
	uint32_t            prevCnt;         // Count of the previous line input
	uint32_t            sameCntSamples;  //

	LineType            lType;           // 1, 2Near, 2Far, ...
	TrackDetectorState  mazeState;       // State of the road signal detecting mazeState machine ()
	TrackType           tType;           // This value will be returned
	LineDirection       lDir;            // Left, Middle, Right
}
LineData;
