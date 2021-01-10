#pragma once

#include <stdint.h>

// Maximum values of the buffers.
#define MAX_VERTEX        (30U)

#define INF_DIST          UINT16_MAX
#define INVALID_VERTEX    UINT8_MAX

typedef uint8_t  VERTEX;
typedef uint8_t  PATH[MAX_VERTEX];
typedef uint16_t GRAPH[MAX_VERTEX][MAX_VERTEX];

typedef enum
{
    eLeft = 0U,
    eMiddle,
    eRight
}
TURN_POSITION;

typedef struct
{
    VERTEX   target;
    uint32_t distance_list[MAX_VERTEX];
    VERTEX   prev_vertex_list[MAX_VERTEX];
}
DIJKSTRA_RESULT;
