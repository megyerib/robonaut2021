#pragma once

#include "MatrixImg.h"

class Matrix
{
public:
	static Matrix* GetInstance();

	void DisplayImage(MATRIX_IMG& img);
	void DisplayInt(int num);
	void Clear();

	static void Refresh();

private:
	Matrix();
};
