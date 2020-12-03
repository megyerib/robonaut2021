#include "Matrix.h"
#include "ShiftReg.h"
#include <cstring>

static uint8_t imgBuf[8] = {0};

static struct
{
	uint8_t ledImg;
	uint8_t col;
	uint8_t row;
}
outBuf;

Matrix::Matrix()
{

}

Matrix& Matrix::GetInstance()
{
	static Matrix instance;
	return instance;
}

void Matrix::DisplayImage(MATRIX_IMG& img)
{
	memcpy(imgBuf, &img, 8);
}

void Matrix::DisplayInt(int num)
{
	const MATRIX_IMG* numbers[10] =
	{
		&img_left0,
		&img_left1,
		&img_left2,
		&img_left3,
		&img_left4,
		&img_left5,
		&img_left6,
		&img_left7,
		&img_left8,
		&img_left9
	};

	uint32_t num1 = num / 10 % 10;
	uint32_t num2 = num % 10;

	if (num1 > 0)
	{
		imgBuf[0] = (*numbers[num1])[0];
		imgBuf[1] = (*numbers[num1])[1];
		imgBuf[2] = (*numbers[num1])[2];
	}
	else
	{
		imgBuf[0] = 0;
		imgBuf[1] = 0;
		imgBuf[2] = 0;
	}

	imgBuf[3] = 0;
	imgBuf[4] = (*numbers[num2])[0];
	imgBuf[5] = (*numbers[num2])[1];
	imgBuf[6] = (*numbers[num2])[2];
	imgBuf[7] = 0;
}

void Matrix::Clear()
{
	memset(imgBuf, 0x00, 8);
}

void Matrix::Refresh()
{
	outBuf.row = imgBuf[rowToRefresh];
	outBuf.col = ~(1 << rowToRefresh);

	ShiftReg::GetInstance().Transmit(&outBuf, 3);

	rowToRefresh++;
	rowToRefresh %= 8;
}
