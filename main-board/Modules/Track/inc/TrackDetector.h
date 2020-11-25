#pragma once

#include "MazeDetectorSM.h"

#define LINE_SAMPLING_CYCLE    5 /* ms */
#define LINE_CNT_FILTER_SIZE   3

typedef enum
{
	Maze,
	Speedrun
}
TrackMode;

class TrackDetector
{
public:
	static TrackDetector* GetInstance();
	void Process(); // For the OS task

	void SetMode(TrackMode mode){this->mode = mode;}
	TrackType GetTrackType(){return front.tType;}
	LineDirection GetLineDirection(){return front.lDir;}
	float GetFrontLine(){return front.pos;} /* m */
	float GetRearLine(){return rear.pos;} /* m */
	float GetFrontLine(LineDirection const dir);
	float GetRearLine(LineDirection const dir);     // TODO pls make it
	bool  IsFrontLineDetected();

	bool IsJunction(TrackType const track);
	bool IsFork(TrackType track);
	LineDirection GetLineDir();

private:
	TrackMode mode = Maze;

	MazeDetectorSM frontStm;

	LineData front = {0};
	LineData rear  = {0};

	TrackDetector();

	// Process:
	bool GetLineData(LineData& line);
	void FilterCnt(LineData& line);

	void EvalRaceTrackType();

	// ---------------------------

	void EvalLineType(LineData& line);
	void GetNearest(LineData& line);
	void GetChosenOne(LineData& line, LineDirection dir);
};
