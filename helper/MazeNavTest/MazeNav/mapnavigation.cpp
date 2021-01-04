#include "mapnavigation.h"
#include <cstring>
#include <cstdio>

#define INF            UINT16_MAX
#define INVALID_VERTEX UINT8_MAX

MapNavigation::MapNavigation()
{
    vertex_count  = 0U;
    memset(graph,    0U, sizeof(graph));
    memset(turnings, 0U, sizeof(turnings));
    source_vertex = 0U;
    target_vertex = 0U;
    actual_vertex = 0U;
    steps         = 0U;
    InitArray<uint8_t>(shortest_path, INVALID_VERTEX, sizeof(shortest_path));
    InitArray<uint8_t>(result.vertex_list, INVALID_VERTEX, sizeof(result.vertex_list));
    InitArray<uint32_t>(result.distance_list, INF, sizeof(result.distance_list));
    InitArray<uint8_t>(result.prev_vertex_list, INVALID_VERTEX, sizeof(result.prev_vertex_list));
}

MAZE_MOVE MapNavigation::GetNextMove(uint8_t target)
{
    MAZE_MOVE next_move;

    if (target_vertex != target)
    {
        target_vertex = target;
        source_vertex = actual_vertex;
        PlanRoute();
    }

    actual_vertex = GetNextVertex();

    next_move.apprDir = APPR_DIR::adForward;
    next_move.exitDir = EXIT_DIR::edFrontRight;
    if (actual_vertex == target_vertex)
    {
        next_move.arrived = true;
    }
    else
    {
        next_move.arrived = false;
    }

    return next_move;
}

void MapNavigation::SetSection(MAZE_SECTION section)
{
    actual_vertex = section;
}

void MapNavigation::InitMap(Edge* const edge_list, uint16_t const node_count)
{
    if(edge_list != nullptr)
    {
        vertex_count = node_count;
        CreateUndirectedAdjacencyMatrixFromEdges(edge_list);
    }
}

void MapNavigation::Dijkstra()
{
    Vertex  selected_vertex = INVALID_VERTEX;
    bool    unvisited_vertices[vertex_count];
    bool    neighbours[vertex_count];
    uint8_t neighbour_count = 0;

    InitArray<uint32_t>(result.distance_list, INF, sizeof(result.distance_list));
    InitArray<uint8_t>(result.prev_vertex_list, INVALID_VERTEX, sizeof(result.prev_vertex_list));
    memset(unvisited_vertices, true, sizeof(unvisited_vertices));

    result.distance_list[source_vertex] = 0;

    while (AllVertexVisited(unvisited_vertices) == false)
    {
        memset(neighbours, false, sizeof(neighbours));
        selected_vertex = FindUnvisitedVertexWithSmallestDistance(unvisited_vertices);
        neighbour_count = CountUnvisitedNeighbours(neighbours, unvisited_vertices, selected_vertex);
        UpdateCurrentNeigbourDistances(neighbour_count, neighbours, selected_vertex);
        unvisited_vertices[selected_vertex] = false;
    }

    for (int i = 0; i < vertex_count; i++)
    {
        result.vertex_list[i] = i;
    }
}

void MapNavigation::PlanRoute()
{
    uint8_t actual_vertex = target_vertex;

    Dijkstra();

    shortest_path[0U] = target_vertex;

    steps = 1U;
    while (actual_vertex != source_vertex)
    {
        actual_vertex = result.prev_vertex_list[actual_vertex];
        shortest_path[steps] = actual_vertex;
        steps++;
    }
}

void MapNavigation::CreateUndirectedAdjacencyMatrixFromEdges(Edge* const edge_list)
{
    Edge* actual_edge = nullptr;
    uint16_t edge_count = sizeof(edge_list);

    for(int i = 0; i < edge_count; i++)
    {
        actual_edge = &edge_list[i];
        graph[actual_edge->start_vertex][actual_edge->end_vertex] = actual_edge->weight;
        graph[actual_edge->end_vertex][actual_edge->start_vertex] = actual_edge->weight;
    }
}

Vertex MapNavigation::FindUnvisitedVertexWithSmallestDistance(bool* const unvisited_vertices)
{
    Vertex selected_vertex = INVALID_VERTEX;
    uint32_t min = INF + 1U;

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

uint8_t MapNavigation::CountUnvisitedNeighbours(bool* neighbours, bool* const unvisited_vertices, const Vertex vertex)
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

bool MapNavigation::AllVertexVisited(bool * const unvisited_vertices)
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

void MapNavigation::UpdateCurrentNeigbourDistances(const uint8_t neighbour_count, bool * const neighbours, Vertex const vertex)
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

Vertex MapNavigation::GetNextVertex()
{
    Vertex next_vertex = INVALID_VERTEX;
    uint8_t index = 0U;

    while (index < steps)
    {
        if (shortest_path[index] == actual_vertex)
        {
            break;
        }

        index++;
    }

    if (index > 1U)
    {
        next_vertex = shortest_path[index - 1U];
    }
    else
    {
        next_vertex = shortest_path[0U];
    }

    return next_vertex;
}

void MapNavigation::PrintfGraph(int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            printf("%d\t", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


