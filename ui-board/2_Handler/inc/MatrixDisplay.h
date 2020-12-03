#pragma once

#include "MatrixImg.h"

class MatrixDisplay
{
public:
	static MatrixDisplay* GetInstance();

	void DisplayImage(MATRIX_IMG& img);
	void DisplayInt(int num);
	void Clear();

	static void Refresh();

private:
	MatrixDisplay();
};
