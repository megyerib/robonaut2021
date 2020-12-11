#include <LsFrontUart.h>
#include <LsRearUart.h>
#include "TrackDetector.h"
#include <cstddef>
#include <cstdlib>
#include "WaitDistance.h"
#include <cstring>
#include "EscapeEncoder.h"
#include "Trace.h"
#include "MainLsMsg.h"
#include "BinTraceBase.h"

#define NEAR_FAR_THRESHOLD_MM    70

#define DEBUG_TRACK               0

#define CMD_PROC_BUF_LEN  300

static uint8_t cmdProcBufFront[CMD_PROC_BUF_LEN];
static uint8_t cmdProcBufRear[CMD_PROC_BUF_LEN];

static EscapeEncoder enc;

TrackDetector::TrackDetector() : frontStm(front),
                                 cmdProcFront(LsFrontUart::GetInstance(), cmdProcBufFront, CMD_PROC_BUF_LEN, &enc),
								 cmdProcRear(LsRearUart::GetInstance(), cmdProcBufRear, CMD_PROC_BUF_LEN, &enc)
{
	front.receiver = &cmdProcFront;
	rear.receiver  = &cmdProcRear;

	/*M2L_RESET reset;

	M2L_CFG cfg;
	cfg.LedEn        = 1;
	cfg.LineDataEn   = 1;
	cfg.MeasEn       = 1;
	cfg.SensorDataEn = 1;

	// TODO do this nicely
	static uint8_t txBuf[10];

	EscapeEncoder enc;
	BinTraceBase frontTrace(
		LsFrontUart::GetInstance(),
		txBuf,
		10,
		10,
		enc
	);

	frontTrace.TraceBinary(false, &cfg, sizeof(cfg));
	frontTrace.Process();*/
}

TrackDetector* TrackDetector::GetInstance()
{
	static TrackDetector instance;
	return &instance;
}

void TrackDetector::GetChosenOne(LineData& line, LineDirection dir)
{
	int32_t index = 0;

	switch (line.input.cnt)
	{
		case 0:
		{
			// prev = prev
			break;
		}
		case 1:
		{
			index = 0;
			break;
		}
		case 2:
		{
			switch (dir)
			{
				case ld_Left:
				{
					index = 0;
					break;
				}
				case ld_Middle:
				{
					index = 0;
					break;
				}
				case ld_Right:
				{
					index = 1;
					break;
				}
				default:
					break;
			}

			break;
		}
		case 3:
		{
			switch (dir)
			{
				case ld_Left:
				{
					index = 0;
					break;
				}
				case ld_Middle:
				{
					index = 1;
					break;
				}
				case ld_Right:
				{
					index = 2;
					break;
				}
				default:
					break;
			}

			break;
		}
		default:
			break;
	}

	line.chosenIndex = index;
	line.pos = line.input.lines[index] / 1000.0f;
}

float TrackDetector::GetFrontLine(LineDirection const dir)
{
	GetChosenOne(front, dir);

	return front.pos;
}

bool TrackDetector::IsJunction(TrackType track)
{
    bool track_is_junction = false;

    if ((track == TrackType::Junction3) || (track == TrackType::Junction2))
    {
        track_is_junction = true;
    }

    return track_is_junction;
}

bool TrackDetector::IsFork(TrackType track)
{
    bool track_is_fork = false;

    if ((track == TrackType::Fork3) || (track == TrackType::Fork2))
    {
        track_is_fork = true;
    }

    return track_is_fork;
}

// Put line data into line.input
bool TrackDetector::ProcessRx(LineData& line)
{
	bool lineReceived = false; // Don't signal if only other types were received
	static uint8_t buf[70];
	size_t size;

	while (true)
	{
		int32_t rxStatus = line.receiver->Receive(buf, size, 70);

		if (rxStatus == RECEIVE_CRC_ERROR)
		{
			PRINTF("Line Rx CRC error");
		}

		if (size == 0) break;

		if (buf[0] == l2mLineData && size == sizeof(L2M_LINE_DATA))
		{
			L2M_LINE_DATA* msg = (L2M_LINE_DATA*) buf;
			line.input = msg->line;
			lineReceived = true;
		}
		else if (buf[0] == l2mSensorData)
		{
			TRACE_BIN(buf, size);
		}
	}

	return lineReceived;
}

void TrackDetector::GetNearest(LineData& line)
{
	if (line.input.cnt > 0)
	{
		int32_t prevLine = (int32_t)(line.pos * 1000);

		int32_t minDiff = abs(line.input.lines[0] - prevLine);
		uint16_t minDiffIndex = 0;

		for (int i = 1; i < line.input.cnt; i++)
		{
			uint16_t diff = abs(line.input.lines[i] - prevLine);

			if (diff < minDiff)
			{
				minDiff = diff;
				minDiffIndex = i;
			}
		}

		line.chosenIndex = minDiffIndex;
		line.pos        = line.input.lines[minDiffIndex] / 1000.0; // mm -> m
	}
}

void TrackDetector::GetMiddle(LineData& line)
{
	if (line.input.cnt == 1)
	{
		line.chosenIndex = 0;
		line.pos         = line.input.lines[0] / 1000.0; // mm -> m
	}
	else if (line.input.cnt == 3)
	{
		line.chosenIndex = 1;
		line.pos         = line.input.lines[1] / 1000.0; // mm -> m
	}
	else
	{
		GetNearest(line);
	}
}

void TrackDetector::FilterCnt(LineData& line)
{
	if (line.prevCnt == line.input.cnt)
	{
		line.sameCntSamples++;

		if (line.sameCntSamples >= LINE_CNT_FILTER_SIZE)
		{
			line.filteredCnt = line.input.cnt;
		}
	}
	else
	{
		line.prevCnt = line.input.cnt;
		line.sameCntSamples = 1;
	}
}

void TrackDetector::EvalRaceTrackType()
{
	static enum {wait_3, slow, accel, fast, brake} state = slow;
	static WaitDistance wait;

	switch (state)
	{
	    case slow: // Single line
        case fast:
        {
            if (front.filteredCnt == 3)
            {
                state = wait_3;
                wait.Wait_m(0); // Just for distance measuring
            }
            break;
        }
	    case wait_3:
        {
            if (front.filteredCnt == 1 && wait.GetDiff() > 0.1f)
            {
                wait.Wait_m(1);
                state = accel;
                front.tType = TrackType::Acceleration;
            }
            else if (wait.GetDiff() > 0.2f)
            {
                wait.Wait_m(2.9);
                state = brake;
                front.tType = TrackType::Braking;
            }

            break;
        }
		case accel:
		{
			if (front.filteredCnt == 1 && wait.IsExpired())
			{
				state = fast;
				front.tType = TrackType::Single;
			}
			break;
		}
		case brake:
		{
			if (front.filteredCnt == 1 && wait.IsExpired())
			{
				state = slow;
				front.tType = TrackType::Single;
			}
			break;
		}
	}
}

void TrackDetector::Process()
{
#if DEBUG_TRACK
	uint16_t frontCnt = 0;
	uint16_t rearCnt  = 0;
#endif

	// Front
	while (ProcessRx(front) == true)
	{
#if DEBUG_TRACK		// Change trace
		LineType  prevLineType = front.lType;
		TrackType prevTrackType = front.tType;
#endif

		FilterCnt(front);
		//GetNearest(front);
		GetMiddle(front);
		EvalLineType(front);

		if (mode == Maze)
		{
			frontStm.Process();
			front.tType = frontStm.GetTrackType();
			front.lDir  = frontStm.GetLineDirection();
		}
		else // Speedrun
		{
			EvalRaceTrackType();
		}

#if DEBUG_TRACK // Change trace
		if (prevLineType != front.lType)
		{
			const char* name = lineTypeNames[front.lType];
			trace->Transmit(name, strlen(name));
		}
		if (prevTrackType != front.tType)
		{
			const char* name = trackTypeNames[front.tType];
			trace->Transmit(name, strlen(name));
		}

		frontCnt++;
#endif
	}

	// Rear
	while (ProcessRx(rear) == true)
	{
		FilterCnt(rear);
		GetNearest(rear);

#if DEBUG_TRACK
		rearCnt++;
#endif
	}
}

bool TrackDetector::IsFrontLineDetected()
{
	if (front.filteredCnt > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TrackDetector::EvalLineType(LineData& line)
{
	if (line.filteredCnt != line.input.cnt)
	{
		// Can't determine line type
		return;
	}

	LineType type = lt_1;

	switch (line.filteredCnt)
	{
		case 2:
		{
			if (abs(line.input.lines[0] - line.input.lines[1]) < NEAR_FAR_THRESHOLD_MM)
			{
				type = lt_2Near;
			}
			else
			{
				type = lt_2Far;
			}
			break;
		}
		case 3:
		{
			if (abs(line.input.lines[0] - line.input.lines[1]) < NEAR_FAR_THRESHOLD_MM ||
				abs(line.input.lines[1] - line.input.lines[2]) < NEAR_FAR_THRESHOLD_MM )
			{
				type = lt_3Near;
			}
			else
			{
				type = lt_3Far;
			}
			break;
		}
		default:
			break;
	}

	if (line.input.cross != 0)
	{
		type = lt_End;
	}

	line.lType = type;
}
