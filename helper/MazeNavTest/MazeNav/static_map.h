#pragma once

#include "mapnavigation.h"

#if USE_STATIC_MAP == 1U
static const uint8_t     cfgVertexCount = 4;

static const TURN_MATRIX cfgTurnMatrix =
{
    {{ 10U, edFrontMid, adBackward}, { 11U, edFrontRight, adForward},  { 12U, edRearRight, adForward},  { 13U, edRearMid, adBackward}},
    {{100U, edFrontMid, adForward},  {111U, edFrontRight, adBackward}, {112U, edRearRight, adBackward}, {113U, edRearMid, adForward}},
    {{210U, edFrontMid, adBackward}, {211U, edFrontRight, adForward},  {212U, edRearRight, adForward},  {213U, edRearMid, adBackward}},
    {{ 10U, edFrontMid, adBackward}, { 11U, edFrontRight, adForward},  { 12U, edRearRight, adForward},  { 13U, edRearMid, adBackward}}
};
#endif
