#pragma once

#include "MatrixImg.h"

#define NO_BLINKING   (0u)

class Matrix
{
public:
	static Matrix& GetInstance();

	void DisplayImage(MATRIX_IMG& img);
	void DisplayInt(int num);
	void Clear();
	void Refresh();
	void SetBlinking(uint32_t period_ms);

private:
	uint32_t rowToRefresh = 0;
	uint32_t blinkCntr = 0;
	uint32_t blinkPeriod = NO_BLINKING;
	bool imgVisible = true;

	Matrix();
};
