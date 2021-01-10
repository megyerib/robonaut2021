#pragma once

#include <stdint.h>
#include "MazeNavBase.h"

// Maximum values of the buffers.
#define MAX_VERTEX   ( 30U)

typedef uint8_t  VERTEX;
typedef uint8_t  PATH[MAX_VERTEX];
typedef uint16_t GRAPH[MAX_VERTEX][MAX_VERTEX];

typedef struct
{
    VERTEX   vertex_in;
    VERTEX   vertex_out;
    uint16_t weight;
    EXIT_DIR turn;
}
TURN_INFO;

typedef struct
{
    uint16_t weight;
    EXIT_DIR turning;
    APPR_DIR direction;
}
MOVE_INFO;

typedef struct
{
    VERTEX   vertex_list[MAX_VERTEX];
    uint32_t distance_list[MAX_VERTEX];
    VERTEX   prev_vertex_list[MAX_VERTEX];
}
DIJKSTRA_RESULT;

typedef struct
{
    TURN_INFO frontLeft;
    TURN_INFO frontMiddle;
    TURN_INFO frontRight;
    TURN_INFO rearLeft;
    TURN_INFO rearMiddle;
    TURN_INFO rearRight;
}
TRUNTABLE;

typedef MOVE_INFO TURN_MATRIX[MAX_VERTEX][MAX_VERTEX];

typedef enum
{
    eLeft = 0U,
    eMiddle,
    eRight
}
TURN_POSITION;

class MapNavigation : public MazeNavBase
{
public:
    MapNavigation();

    virtual MAZE_MOVE GetNextMove(uint8_t target) override;
    virtual void SetSection(MAZE_SECTION section) override;

    void PrintfGraph(int size);

    void InitMap(uint16_t const node_count);
    void AddJunction(TRUNTABLE const junction);
    void RegisterTurns(TURN_INFO const from,
                       TURN_INFO const to,
                       TURN_POSITION const tpos_from,
                       TURN_POSITION const tpos_to);
    void PrintTrunMatrix(int size);

    void PrintPathMoves(int size);

private:
    uint16_t   vertex_count;
    GRAPH      graph;
    VERTEX     source_vertex;
    VERTEX     target_vertex;
    VERTEX     actual_vertex;
    uint8_t    steps;
    PATH       shortest_path;
    DIJKSTRA_RESULT result;

    TURN_MATRIX turnMatrix;

    void Dijkstra();

    void PlanRoute();

    VERTEX FindUnvisitedVertexWithSmallestDistance(bool* const unvisited_vertices);

    uint8_t CountUnvisitedNeighbours(bool* neighbours, bool* const unvisited_vertices, VERTEX const vertex);

    bool AllVertexVisited(bool* const unvisited_vertices);

    void UpdateCurrentNeigbourDistances(uint8_t const neighbour_count, bool* const neighbours, VERTEX const vertex);

    VERTEX GetNextVertex();

    template<typename T>
    void InitArray(T* array, T value, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            array[i] = value;
        }
    }

    bool IsTurnInfoValid(TURN_INFO const turn_info);
};

