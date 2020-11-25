#include "TrackDetector.h"
#include <cstddef>
#include "base64.h" /* Change encoding & remove */
#include <cstdlib>
#include "WaitDistance.h"

#include "LsUartFront.h"
#include "LsUartRear.h"
#include "LastLineReader.h"
#include "LineGetterUart.h"
#include <cstring>

#define NEAR_FAR_THRESHOLD_MM    70

#define DEBUG_TRACK               0

TrackDetector::TrackDetector() :
	frontStm(front)
{
	front.receiver = LsUartFront::GetInstance();
	rear.receiver  = LsUartRear ::GetInstance();
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

bool TrackDetector::GetLineData(LineData& line)
{
	uint8_t base64_buf[50];
	size_t  base64_size;
	bool ret = false;

	line.receiver->Receive(base64_buf, base64_size, 50);

	while (base64_size > 0)
	{
		if (base64_size == 11)
		{
			uint8_t decoded_buf[11];
			size_t  decoded_size;

			base64_decode(base64_buf, decoded_buf, base64_size, &decoded_size);

			if (decoded_size == sizeof(LineInput))
			{
				line.input = *((LineInput*) decoded_buf);
				ret = true;
				break;
			}
		}

		line.receiver->Receive(base64_buf, base64_size, 50);
	}

	return ret;
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
	while (GetLineData(front) == true)
	{
#if DEBUG_TRACK		// Change trace
		LineType  prevLineType = front.lType;
		TrackType prevTrackType = front.tType;
#endif

		FilterCnt(front);
		GetNearest(front);
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
	while (GetLineData(rear) == true)
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
