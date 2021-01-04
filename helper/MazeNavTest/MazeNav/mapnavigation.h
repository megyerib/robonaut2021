#pragma once

#include <stdint.h>
#include "MazeNavBase.h"

// Maximum values of the buffers.
#define MAX_VERTEX   ( 40U)
#define MAX_EDGES    ( 60U)
#define MAX_TURNINFO (120U)

typedef uint8_t  Vertex;
typedef uint8_t  Path[MAX_VERTEX];
typedef uint32_t Graph[MAX_VERTEX][MAX_VERTEX];

typedef struct
{
    Vertex   start_vertex;
    Vertex   end_vertex;
    uint32_t weight;

    void Init(Vertex s, Vertex e, uint32_t w)
    {
        start_vertex = s;
        end_vertex = e;
        weight = w;
    }
}
Edge;

typedef struct
{
    Vertex   from_vertex;
    Vertex   to_vertex;
    APPR_DIR direction;
    EXIT_DIR turning;
}
TurnTable;

typedef struct
{
    Vertex   vertex_list[MAX_VERTEX];
    uint32_t distance_list[MAX_VERTEX];
    Vertex   prev_vertex_list[MAX_VERTEX];
}
DijkstraResult;

class MapNavigation : public MazeNavBase
{
public:
    MapNavigation();

    virtual MAZE_MOVE GetNextMove(uint8_t target) override;
    virtual void SetSection(MAZE_SECTION section) override;

    void InitMap(Edge* const edge_list, uint16_t const node_count);
    void InitTurnTable(TurnTable* const turn_table);
    void PrintfGraph(int size);

private:
    uint16_t   vertex_count;
    Graph      graph;
    TurnTable  turnings[MAX_TURNINFO];
    Vertex     source_vertex;
    Vertex     target_vertex;
    Vertex     actual_vertex;
    uint8_t    steps;
    Path       shortest_path;
    DijkstraResult result;

    void Dijkstra();
    void PlanRoute();
    void CreateUndirectedAdjacencyMatrixFromEdges(Edge* const edge_list);
    Vertex FindUnvisitedVertexWithSmallestDistance(bool* const unvisited_vertices);
    uint8_t CountUnvisitedNeighbours(bool* neighbours, bool* const unvisited_vertices, Vertex const vertex);
    bool AllVertexVisited(bool* const unvisited_vertices);
    void UpdateCurrentNeigbourDistances(uint8_t const neighbour_count, bool* const neighbours, Vertex const vertex);
    Vertex GetNextVertex();

    template<typename T>
    void InitArray(T* array, T value, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            array[i] = value;
        }
    }

};

