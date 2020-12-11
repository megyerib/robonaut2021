#pragma once

#include <cstdint>
#include <cstddef>

#define RECEIVE_OK                        ( 0)
#define RECEIVE_PARAM_ERROR               (-1)
#define RECEIVE_INSUFFICIENT_TARGET_SIZE  (-2)
#define RECEIVE_INTERNAL_ERROR            (-3)
#define RECEIVE_CRC_ERROR                 (-4)

class Receiver
{
public:
	virtual int32_t Receive(void* buffer, size_t& size, size_t targetSize) = 0;
};
