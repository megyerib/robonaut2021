#pragma once

#include <cstddef> /* size_t */
#include <cstdint>

class BinaryEncoder
{
public:
    virtual bool Encode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize, size_t dstMax) = 0;
    virtual bool Decode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize, size_t dstMax) = 0;
};
