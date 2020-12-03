#pragma once

#include <cstdint>
#include <cstddef>

#define TRANSMIT_OK                        ( 0)
#define TRANSMIT_BUFFER_NULL               (-1)
#define TRANSMIT_INSUFFICIENT_TARGET_SIZE  (-2)
#define TRANSMIT_INTERNAL_ERROR            (-3)
#define TRANSMIT_BUSY                      (-4)
#define TRANSMIT_DISABLED                  (-5)

class Transmitter
{
public:
	virtual int32_t Transmit(const void* buffer, size_t size) = 0;
};
