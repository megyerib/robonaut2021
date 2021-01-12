#pragma once

#include <stdint.h>

// Settings

#define BACKWARD_PENALTY         (1.10F)
#define DIRECTION_CHANGE_PENALTY (1.05F)

#define USE_STATIC_MAP           (0U)

// Maximum values of the buffers.
#define MAX_VERTEX        (48U)

#define INF_DIST          (UINT16_MAX-1U)
#define INVALID_VERTEX    (UINT8_MAX)

typedef uint8_t  VERTEX;
typedef uint8_t  PATH[MAX_VERTEX];
typedef uint16_t GRAPH[MAX_VERTEX][MAX_VERTEX];

typedef enum : uint8_t
{
    tpLeft = 0U,
    tpMiddle,
    tpRight
}
TURN_POSITION;

typedef struct
{
    VERTEX   target;
    uint16_t distance_list[MAX_VERTEX];
    VERTEX   prev_vertex_list[MAX_VERTEX];
}
DIJKSTRA_RESULT;
