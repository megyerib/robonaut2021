#include "StddevEval.h"
#include <string.h>
#include <math.h>
#include <SensorCfg.h>

#define THRESHOLD         200
#define BLACK_TRESHOLD   1500
#define CROSS_BLACK_NUM    10

StddevEval::StddevEval()
{

}

void StddevEval::Feed(AdcMeasType meas[SENSOR_SIZE])
{
	memcpy(data, meas, sizeof(data));
}

LineInput StddevEval::GetLine()
{
	uint32_t filtered[SENSOR_SIZE];
	uint32_t i;
	uint32_t avg;
	uint32_t stdDev;
	uint32_t lineCnt = 0;

	LineInput ret;

	// Filtering
	magicDiff(data, filtered);

	// Average, standard deviation
	avg = mean(filtered, SENSOR_SIZE);
	stdDev = standardDeviation(filtered, SENSOR_SIZE, avg);

	// Sensor line
	for (i = 0; i < SENSOR_SIZE; i++)
	{
		if (evalIsPeak(filtered, i, avg, stdDev))
		{
			if (lineCnt < MAXLINES)
			{
				ret.lines[lineCnt] = evalWeightedMean(filtered, i);
			}

			lineCnt++;
		}
	}

	// Count
	ret.cnt = lineCnt > MAXLINES ? MAXLINES : lineCnt;

	// Cross
	uint32_t blackcnt = 0;

	for (int i = 0; i < SENSOR_SIZE; i++)
	{
		if (data[i] >= BLACK_TRESHOLD)
		{
			blackcnt++;
		}
	}

	ret.cross = (blackcnt >= CROSS_BLACK_NUM) ? 1 : 0;

	// Return
	return ret;
}

// A 4 legközelebbi szomszédtól vett legnagyobb különbség, ha pozitív, egyébként 0.
void StddevEval::magicDiff(uint32_t* src, uint32_t* dst)
{
	uint32_t padded[SENSOR_SIZE + 4]; // Two padding values with value 0 on each side

    memcpy(&padded[2], src, SENSOR_SIZE * sizeof(uint32_t));

    padded[0] = padded[1] = padded[2];
    padded[SENSOR_SIZE + 3] = padded[SENSOR_SIZE + 2] = padded[SENSOR_SIZE + 1];

    // padded | 0 | 1 | 2 | 3 | 4 | 5 | ...
    //          *   *   *   *   *
    //                  *
    // dst            | 0 | 1 | 2 | 3 | ...

    for (int i = 0; i < SENSOR_SIZE; i++)
	{
		dst[i] = max4_pos(
			padded[i+2] - padded[i],
			padded[i+2] - padded[i+1],
			padded[i+2] - padded[i+3],
			padded[i+2] - padded[i+4]
		);
	}
}

// A 4 érték közül a legnagyobbal tér vissza, ha negatív, akkor 0-val.
uint32_t StddevEval::max4_pos(int32_t arg1, int32_t arg2, int32_t arg3, int32_t arg4)
{
    int32_t max = 0;

    max = arg1 > max ? arg1 : max;
    max = arg2 > max ? arg2 : max;
    max = arg3 > max ? arg3 : max;
    max = arg4 > max ? arg4 : max;

    return (uint32_t) max;
}

int16_t StddevEval::ledPosToMm(uint8_t ledPos)
{
    int16_t mm = (ledPos * IR_DIST_MM) - MID_IR_POS_MM;
    return mm;
}

int32_t StddevEval::evalWeightedMean(uint32_t arr[SENSOR_SIZE], uint32_t i)
{
    int32_t w1, w2, w3, div, ret;

    uint32_t padded[SENSOR_SIZE + 2] = {0};
    memcpy(&padded[1], arr, SENSOR_SIZE * sizeof(uint32_t));

    // padded | 0 | 1 | 2 | 3 | ...
	//          *   *   *
	//              *
	// arr        | 0 | 1 | 2 | ...

	w1 = padded[i+0] * ledPosToMm(i-1);
	w2 = padded[i+1] * ledPosToMm(i+0);
	w3 = padded[i+2] * ledPosToMm(i+1);

	div = padded[i+0] + padded[i+1] + padded[i+2];

    ret = (w1 + w2 + w3) / div;

    return ret;
}

// Evaluates if there is a peak on the point specified by 'i'
uint32_t StddevEval::evalIsPeak(uint32_t* arr, uint32_t i, uint32_t mean, uint32_t stdDev)
{
    // Threshold
    if (arr[i] < mean + stdDev)
    {
        return 0;
    }

    // Peak
    if (i == 0)
    {
        return (arr[1]  + THRESHOLD) < arr[0];
    }
    else if (i == 31)
    {
        return (arr[SENSOR_SIZE - 2] + THRESHOLD) < arr[SENSOR_SIZE - 1];
    }
    else
    {
        return
            ( ((arr[i-1] + THRESHOLD) < arr[i]) && (arr[i+1] < arr[i]) )
            ||
            ( ((arr[i+1] + THRESHOLD) < arr[i]) && (arr[i-1] < arr[i]) );
    }
}

uint32_t StddevEval::mean(uint32_t* data, uint32_t num)
{
    uint32_t sum = 0, i;

    for (i = 0; i < num; i++)
        sum += data[i];

    return sum / num;
}

uint32_t StddevEval::standardDeviation(uint32_t* data, uint32_t num, uint32_t avg)
{
    uint32_t sqsum = 0, var, i;

    for (i = 0; i < num; i++)
        sqsum += (data[i] - avg) * (data[i] - avg);

    var = sqsum / num;

    return sqrt(var);
}
