#pragma once

#include "MazeDetectorSM.h"
#include "CmdReceiver.h"

#define LINE_SAMPLING_CYCLE    5 /* ms */
#define LINE_CNT_FILTER_SIZE   3

typedef enum
{
	trackMaze,
	trackSpeedrun
}
TrackMode;

class TrackDetector
{
public:
	static TrackDetector* GetInstance();
	void Process(); // For the OS task

	void SetMode(TrackMode mode){this->mode = mode;}
	TrackType GetTrackType(){return front.tType;}
	float GetFrontLine(){return front.pos;} /* m */
	float GetRearLine(){return rear.pos;} /* m */
	float GetFrontLine(LineDirection const dir);
	float GetRearLine(LineDirection const dir);

	bool IsJunction(TrackType const track);
	bool IsFork(TrackType track);
	LineDirection GetLineDir();

private:
	TrackMode mode = trackMaze;

	MazeDetectorSM frontStm;

	LineData front = {0};
	LineData rear  = {0};

	CmdReceiver cmdProcFront;
	CmdReceiver cmdProcRear;

	TrackDetector();

	// Process:
	bool ProcessRx(LineData& line);
	void FilterCnt(LineData& line);

	void EvalRaceTrackType();

	// ---------------------------

	void EvalLineType(LineData& line);
	void GetNearest(LineData& line);
	void GetMiddle(LineData& line);
	void GetChosenOne(LineData& line, LineDirection dir);
};
