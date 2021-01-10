#pragma once

#include "map_types.h"

// ON = 1U, OFF = 0U
#define DEBUG_DIJKTSRA_FUNC_ON   (0U)

class Dijkstra
{
public:
    Dijkstra();

    void CalculatePath(VERTEX const source);
    void CalculatePath(VERTEX const target_pos, VERTEX const target_neg);

    DIJKSTRA_RESULT GetResult();

    void InitGraph(uint8_t const vertex_count);

    void AddEdge(VERTEX const from, VERTEX const to, uint16_t const distance);

#if DEBUG_DIJKTSRA_FUNC_ON == 1U
    void PrintfGraph(int size);
#endif

private:
    uint16_t        vertex_count;
    GRAPH           graph;
    VERTEX          source_vertex;
    VERTEX          target_vertex;
    VERTEX          target_vertex_pos;
    VERTEX          target_vertex_neg;
    DIJKSTRA_RESULT result;


    // Shortest path algorithms.
    void Algoritm_Full();

    void Algorithm_UntilTargets(VERTEX const target_pos, VERTEX const target_neg);


    // Helper functions.
    bool AllVertexVisited(bool* const unvisited_vertices);

    VERTEX  FindUnvisitedVertexWithSmallestDistance(bool* const unvisited_vertices);

    uint8_t CountUnvisitedNeighbours(bool* neighbours, bool* const unvisited_vertices, VERTEX const vertex);

    void    UpdateCurrentNeigbourDistances(uint8_t const neighbour_count, bool* const neighbours, VERTEX const vertex);

    template<typename T>
    void InitArray(T* array, T value, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            array[i] = value;
        }
    }
};

