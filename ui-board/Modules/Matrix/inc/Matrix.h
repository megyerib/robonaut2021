#pragma once

#include "MatrixImg.h"

class Matrix
{
public:
	static Matrix& GetInstance();

	void DisplayImage(MATRIX_IMG& img);
	void DisplayInt(int num);
	void Clear();
	void Refresh();

private:
	uint32_t rowToRefresh = 0;

	Matrix();
};
