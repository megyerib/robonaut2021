#pragma once

#include <stdint.h>
#include "MazeNavBase.h"

// Maximum values of the buffers.
#define MAX_VERTEX   ( 40U)
#define MAX_EDGES    ( 60U)
#define MAX_TURNINFO (600U)

typedef uint8_t  VERTEX;
typedef uint8_t  PATH[MAX_VERTEX];
typedef uint32_t GRAPH[MAX_VERTEX][MAX_VERTEX];

typedef struct
{
    VERTEX   start_vertex;
    VERTEX   end_vertex;
    uint32_t weight;

    void Init(VERTEX s, VERTEX e, uint32_t w)
    {
        start_vertex = s;
        end_vertex = e;
        weight = w;
    }
}
EDGE;

typedef struct
{
    VERTEX   from_vertex;
    VERTEX   to_vertex;
    APPR_DIR direction;
    EXIT_DIR turning;
}
TURNTABLE;

typedef struct
{
    VERTEX   vertex_list[MAX_VERTEX];
    uint32_t distance_list[MAX_VERTEX];
    VERTEX   prev_vertex_list[MAX_VERTEX];
}
DIJKSTRA_RESULT;

class MapNavigation : public MazeNavBase
{
public:
    MapNavigation();

    virtual MAZE_MOVE GetNextMove(uint8_t target) override;
    virtual void SetSection(MAZE_SECTION section) override;

    void InitMap(EDGE* const edge_list, uint16_t const node_count, uint16_t const edge_count);
    void InitTurnTable(TURNTABLE* const turn_table);
    void PrintfGraph(int size);

private:
    uint16_t   vertex_count;
    GRAPH      graph;
    TURNTABLE  turnings[MAX_TURNINFO];
    VERTEX     source_vertex;
    VERTEX     target_vertex;
    VERTEX     actual_vertex;
    uint8_t    steps;
    PATH       shortest_path;
    DIJKSTRA_RESULT result;

    void Dijkstra();
    void PlanRoute();
    void CreateUndirectedAdjacencyMatrixFromEdges(EDGE* const edge_list, uint16_t const edge_count);
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

};

