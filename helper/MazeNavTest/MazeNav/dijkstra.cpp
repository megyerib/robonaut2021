#include "dijkstra.h"
#include <cstring>
#include <cstdio>

Dijkstra::Dijkstra()
{
    memset(graph, 0U, sizeof(graph));
    InitArray<uint8_t>(result.vertex_list, INVALID_VERTEX, sizeof(result.vertex_list));
    InitArray<uint32_t>(result.distance_list, INF_DIST, sizeof(result.distance_list)/sizeof (result.distance_list[0]));
    InitArray<uint8_t>(result.prev_vertex_list, INVALID_VERTEX, sizeof(result.prev_vertex_list));
}

void Dijkstra::CalculatePath(const VERTEX source)
{
    source_vertex = source;

    Algoritm_Full();
}

void Dijkstra::CalculatePath(const VERTEX target_pos, const VERTEX target_neg)
{
    // TODO: Implementation.
    (void)target_pos;
    (void)target_neg;
}

DIJKSTRA_RESULT Dijkstra::GetResult()
{
    return result;
}

void Dijkstra::InitGraph(const uint8_t vertex_count)
{
    this->vertex_count = vertex_count;
}

void Dijkstra::AddEdge(const VERTEX from, const VERTEX to, const uint16_t distance)
{
    graph[from][to] = distance;
}

#if DEBUG_DIJKTSRA_FUNC_ON == 1U
void Dijkstra::PrintfGraph(int size)
{
    for (int i = 0U; i < size; i++)
    {
        for (int j = 0U; j < size; j++)
        {
            printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
#endif

void Dijkstra::Algoritm_Full()
{
    VERTEX  selected_vertex = INVALID_VERTEX;
    bool    unvisited_vertices[vertex_count];
    bool    neighbours[vertex_count];
    uint8_t neighbour_count = 0U;

    InitArray<uint32_t>(result.distance_list, INF_DIST, sizeof(result.distance_list)/sizeof(result.distance_list[0]));
    InitArray<uint8_t>(result.prev_vertex_list, INVALID_VERTEX, sizeof(result.prev_vertex_list));
    memset(unvisited_vertices, true, sizeof(unvisited_vertices));

    result.distance_list[source_vertex] = 0U;

    while (AllVertexVisited(unvisited_vertices) == false)
    {
        memset(neighbours, false, sizeof(neighbours));
        selected_vertex = FindUnvisitedVertexWithSmallestDistance(unvisited_vertices);
        neighbour_count = CountUnvisitedNeighbours(neighbours, unvisited_vertices, selected_vertex);
        UpdateCurrentNeigbourDistances(neighbour_count, neighbours, selected_vertex);
        unvisited_vertices[selected_vertex] = false;
    }

    for (int i = 0U; i < vertex_count; i++)
    {
        result.vertex_list[i] = i;
    }
}

void Dijkstra::Algorithm_UntilTargets(VERTEX const target_pos, VERTEX const target_neg)
{
    // TODO: Implementation.
    (void)target_pos;
    (void)target_neg;
}

bool Dijkstra::AllVertexVisited(bool * const unvisited_vertices)
{
    bool allVisited = true;

    for (int i = 0U; i < vertex_count; i++)
    {
        if (unvisited_vertices[i] == true)
        {
            allVisited = false;
            break;
        }
    }

    return allVisited;
}

VERTEX Dijkstra::FindUnvisitedVertexWithSmallestDistance(bool * const unvisited_vertices)
{
    VERTEX selected_vertex = INVALID_VERTEX;
    uint32_t min = INF_DIST + 1U;

    for (int i = 0U; i < vertex_count; i++)
    {
        if ((unvisited_vertices[i] == true) && (result.distance_list[i] < min))
        {
            min = result.distance_list[i];
            selected_vertex = i;
        }
    }

    return selected_vertex;
}

uint8_t Dijkstra::CountUnvisitedNeighbours(bool *neighbours, bool * const unvisited_vertices, const VERTEX vertex)
{
    uint8_t count = 0U;
    memset(neighbours, false, sizeof(neighbours[0U]));

    for (int neighbour = 0U; neighbour < vertex_count; neighbour++)
    {
        if ((unvisited_vertices[neighbour] == true) && (graph[vertex][neighbour] > 0U))
        {
            count++;
            neighbours[neighbour] = true;
        }
    }

    return count;
}

void Dijkstra::UpdateCurrentNeigbourDistances(const uint8_t neighbour_count, bool * const neighbours, const VERTEX vertex)
{
    uint8_t n = 0U;

    for (int next_v = 0U; next_v < vertex_count; next_v++)
    {
        if (neighbours[next_v] == true)
        {
            uint32_t alternative_dist = result.distance_list[vertex] + graph[vertex][next_v];

            if (alternative_dist < result.distance_list[next_v])
            {
                result.distance_list[next_v] = alternative_dist;
                result.prev_vertex_list[next_v] = vertex;
            }

            n++;
        }

        if (n == neighbour_count)
        {
            break;
        }
    }
}
