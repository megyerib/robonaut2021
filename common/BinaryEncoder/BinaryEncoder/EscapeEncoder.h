#pragma once

#include "BinaryEncoder.h"

class EscapeEncoder : public BinaryEncoder
{
public:
    virtual bool Encode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize, size_t dstMax) override;
    virtual bool Decode(const uint8_t* src, size_t srcSize, uint8_t* dst, size_t& dstSize, size_t dstMax) override;
};
