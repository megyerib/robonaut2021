#pragma once

// Trace on = leave defined, Trace off = comment out.
//#define OVERTAKE_TRACE_ENABLED

// Maneuver settings
// Q2
#define OVERTAKE_PREPARE_WAIT_DIST         ( 0.050f)   // m
#define OVERTAKE_PREPARE_SPEED             ( 1.500f)   // m/s
#define OVERTAKE_PREPARE_WHEEL_ANGLE       ( 0.000f)   // rad

#define OVERTAKE_LEAVE_LINE_WAIT_DIST      ( 0.500f)   // m
#define OVERTAKE_LEAVE_LINE_SPEED          ( 1.800f)   // m/s
#define OVERTAKE_LEAVE_LINE_WHEEL_ANGLE    ( 0.349f)   // rad

#define OVERTAKE_GET_PARALLEL_WAIT_DIST    ( 0.500f)   // m
#define OVERTAKE_GET_PARALLEL_SPEED        ( 1.800f)   // m/s
#define OVERTAKE_GET_PARALLEL_WHEEL_ANGLE  (-0.349f)   // rad

#define OVERTAKE_PASS_CAR_WAIT_DIST        ( 4.000f)   // m
#define OVERTAKE_PASS_CAR_SPEED            ( 3.600f)   // m/s
#define OVERTAKE_PASS_CAR_WHEEL_ANGLE      ( 0.000f)   // rad

#define OVERTAKE_TURN_TO_LINE_WAIT_DIST    ( 0.400f)   // m
#define OVERTAKE_TURN_TO_LINE_SPEED        ( 1.800f)   // m/s
#define OVERTAKE_TURN_TO_LINE_WHEEL_ANGLE  (-0.436f)   // rad

#define OVERTAKE_SEARCH_LINE_WAIT_DIST     ( 1.000f)   // m
#define OVERTAKE_SEARCH_LINE_SPEED         ( 1.500f)   // m/s
#define OVERTAKE_SEARCH_LINE_WHEEL_ANGLE   ( 0.218f)   // rad
