#include "mapnavigation.h"
#include <cstring>
#include <cstdio>

#define INF            UINT16_MAX
#define INVALID_VERTEX UINT8_MAX

MapNavigation::MapNavigation()
{
    vertex_count  = 0U;
    memset(graph,    0U, sizeof(graph));
    source_vertex = 0U;
    target_vertex = 0U;
    actual_vertex = 0U;
    steps         = 0U;
    InitArray<uint8_t>(shortest_path, INVALID_VERTEX, sizeof(shortest_path));
    InitArray<uint8_t>(result.vertex_list, INVALID_VERTEX, sizeof(result.vertex_list));
    InitArray<uint32_t>(result.distance_list, INF, sizeof(result.distance_list));
    InitArray<uint8_t>(result.prev_vertex_list, INVALID_VERTEX, sizeof(result.prev_vertex_list));
    memset(turnMatrix, -1, sizeof(turnMatrix));
}

MAZE_MOVE MapNavigation::GetNextMove(uint8_t target)
{
    MAZE_MOVE next_move;
    VERTEX prev_vertex;

    if (target_vertex != target)
    {
        target_vertex = target;
        source_vertex = actual_vertex;
        PlanRoute();
    }

    prev_vertex = actual_vertex;
    actual_vertex = GetNextVertex();

    next_move.apprDir = turnMatrix[prev_vertex][actual_vertex].direction;
    next_move.exitDir = turnMatrix[prev_vertex][actual_vertex].turning;

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

void MapNavigation::InitMap(EDGE* const edge_list, uint16_t const node_count, uint16_t const edge_count)
{
    if(edge_list != nullptr)
    {
        vertex_count = node_count;
        CreateUndirectedAdjacencyMatrixFromEdges(edge_list, edge_count);
    }
}

void MapNavigation::Dijkstra()
{
    VERTEX  selected_vertex = INVALID_VERTEX;
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

void MapNavigation::CreateUndirectedAdjacencyMatrixFromEdges(EDGE* const edge_list, uint16_t const edge_count)
{
    EDGE* actual_edge = nullptr;

    for(int i = 0; i < edge_count; i++)
    {
        actual_edge = &edge_list[i];
        graph[actual_edge->start_vertex][actual_edge->end_vertex] = actual_edge->weight;
        graph[actual_edge->end_vertex][actual_edge->start_vertex] = actual_edge->weight;
    }
}

VERTEX MapNavigation::FindUnvisitedVertexWithSmallestDistance(bool* const unvisited_vertices)
{
    VERTEX selected_vertex = INVALID_VERTEX;
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

uint8_t MapNavigation::CountUnvisitedNeighbours(bool* neighbours, bool* const unvisited_vertices, const VERTEX vertex)
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

void MapNavigation::UpdateCurrentNeigbourDistances(const uint8_t neighbour_count, bool * const neighbours, VERTEX const vertex)
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

VERTEX MapNavigation::GetNextVertex()
{
    VERTEX next_vertex = INVALID_VERTEX;
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

void MapNavigation::InitMap(const uint16_t node_count)
{
    vertex_count = node_count;
}

void MapNavigation::AddJunction(const TRUNTABLE junction)
{

}

bool MapNavigation::IsTurnInfoValid(const TURN_INFO turn_info)
{
    bool isValid = false;

    if ((turn_info.vertex_in != INVALID_VERTEX) &&
        (turn_info.vertex_out != INVALID_VERTEX) &&
        (turn_info.weight >= 0U))   // TODO
    {
        isValid = true;
    }

    return isValid;
}

void MapNavigation::RegisterTurns(const TURN_INFO from, const TURN_INFO to, const TURN_POSITION tpos_from, const TURN_POSITION tpos_to)
{
    bool isFromFront = false;
    bool isToFront   = false;
    uint16_t distance = from.weight + to.weight;

    if (from.turn == EXIT_DIR::edFrontLeft || from.turn == EXIT_DIR::edFrontMid || from.turn == EXIT_DIR::edFrontRight)
    {
        isFromFront = true;
    }

    if (to.turn == EXIT_DIR::edFrontLeft || to.turn == EXIT_DIR::edFrontMid || to.turn == EXIT_DIR::edFrontRight)
    {
        isToFront = true;
    }

    // L\|M/R
    //    x
    // L/|M\R
    if (isFromFront == isToFront)
    {
        // From <- To
        turnMatrix[to.vertex_in][from.vertex_in].direction   = APPR_DIR::adForward;
        turnMatrix[to.vertex_out][from.vertex_out].direction = APPR_DIR::adBackward;

        turnMatrix[to.vertex_in][from.vertex_in].weight   = distance;
        turnMatrix[to.vertex_out][from.vertex_out].weight = distance;

        switch (tpos_from)
        {
            case TURN_POSITION::eLeft:
            {
                if (isFromFront == true)
                {
                    turnMatrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turnMatrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontLeft;
                }
                else
                {
                    turnMatrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turnMatrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                break;
            }
            case TURN_POSITION::eMiddle:
            {
                turnMatrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearMid;
                turnMatrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontMid;
                break;
            }
            case TURN_POSITION::eRight:
            {
                if (isFromFront == true)
                {
                    turnMatrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turnMatrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                else
                {
                    turnMatrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turnMatrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontLeft;
                }
                break;
            }
            default:
            {
                // Error.
                break;
            }
        }

        // From -> To
        turnMatrix[from.vertex_in][to.vertex_in].direction   = APPR_DIR::adForward;
        turnMatrix[from.vertex_out][to.vertex_out].direction = APPR_DIR::adBackward;

        turnMatrix[from.vertex_in][to.vertex_in].weight   = distance;
        turnMatrix[from.vertex_out][to.vertex_out].weight = distance;

        switch (tpos_to)
        {
            case TURN_POSITION::eLeft:
            {
                if (isFromFront == true)
                {
                    turnMatrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turnMatrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontLeft;
                }
                else
                {
                    turnMatrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turnMatrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                break;
            }
            case TURN_POSITION::eMiddle:
            {
                turnMatrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearMid;
                turnMatrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontMid;
                break;
            }
            case TURN_POSITION::eRight:
            {
                if (isFromFront == true)
                {
                    turnMatrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turnMatrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                else
                {
                    turnMatrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turnMatrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontLeft;
                }
                break;
            }
            default:
            {
                // Error.
                break;
            }
        }
    }
    else
    {
        // From <- To
        turnMatrix[from.vertex_in][to.vertex_out].direction = APPR_DIR::adForward;
        turnMatrix[from.vertex_out][to.vertex_in].direction = APPR_DIR::adBackward;

        turnMatrix[from.vertex_in][to.vertex_out].weight = distance;
        turnMatrix[from.vertex_out][to.vertex_in].weight = distance;

        switch (tpos_from)
        {
            case TURN_POSITION::eLeft:
            {
                turnMatrix[to.vertex_in][from.vertex_out].turning = EXIT_DIR::edFrontLeft;
                turnMatrix[to.vertex_out][from.vertex_in].turning = EXIT_DIR::edRearRight;
                break;
            }
            case TURN_POSITION::eMiddle:
            {
                turnMatrix[to.vertex_in][from.vertex_out].turning = EXIT_DIR::edFrontMid;
                turnMatrix[to.vertex_out][from.vertex_in].turning = EXIT_DIR::edRearMid;
                break;
            }
            case TURN_POSITION::eRight:
            {
                turnMatrix[to.vertex_in][from.vertex_out].turning = EXIT_DIR::edFrontRight;
                turnMatrix[to.vertex_out][from.vertex_in].turning = EXIT_DIR::edRearLeft;
                break;
            }
            default:
            {
                // Error.
                break;
            }
        }

        // From -> To
        turnMatrix[to.vertex_in][from.vertex_out].direction = APPR_DIR::adForward;
        turnMatrix[to.vertex_out][from.vertex_in].direction = APPR_DIR::adBackward;

        turnMatrix[to.vertex_in][from.vertex_out].weight = distance;
        turnMatrix[to.vertex_out][from.vertex_in].weight = distance;

        switch (tpos_to)
        {
            case TURN_POSITION::eLeft:
            {
                turnMatrix[from.vertex_in][to.vertex_out].turning = EXIT_DIR::edFrontRight;
                turnMatrix[from.vertex_out][to.vertex_in].turning = EXIT_DIR::edRearLeft;
                break;
            }
            case TURN_POSITION::eMiddle:
            {
                turnMatrix[from.vertex_in][to.vertex_out].turning = EXIT_DIR::edFrontMid;
                turnMatrix[from.vertex_out][to.vertex_in].turning = EXIT_DIR::edRearMid;
                break;
            }
            case TURN_POSITION::eRight:
            {
                turnMatrix[from.vertex_in][to.vertex_out].turning = EXIT_DIR::edFrontLeft;
                turnMatrix[from.vertex_out][to.vertex_in].turning = EXIT_DIR::edRearRight;
                break;
            }
            default:
            {
                // Error.
                break;
            }
        }
    }
}

void MapNavigation::PrintTrunMatrix(int size)
{
    for (int i = 1; i < size; i++)
    {
        for (int j = 1; j < size; j++)
        {
            if (turnMatrix[i][j].weight == 65535)
            {
                printf("[0, 0, 00]\t");
            }
            else
            {
                char d = '0';
                if (turnMatrix[i][j].direction == APPR_DIR::adBackward){    d = 'B';    }
                else { d = 'F'; }

                char t[3] = "00";
                if (turnMatrix[i][j].turning == EXIT_DIR::edFrontLeft){         t[0] = 'F'; t[1] = 'L';   }
                else if (turnMatrix[i][j].turning == EXIT_DIR::edFrontMid){     t[0] = 'F'; t[1] = 'M';   }
                else if (turnMatrix[i][j].turning == EXIT_DIR::edFrontRight){   t[0] = 'F'; t[1] = 'R';   }
                else if (turnMatrix[i][j].turning == EXIT_DIR::edRearLeft){     t[0] = 'R'; t[1] = 'L';   }
                else if (turnMatrix[i][j].turning == EXIT_DIR::edRearMid){      t[0] = 'R'; t[1] = 'M';   }
                else if (turnMatrix[i][j].turning == EXIT_DIR::edRearRight){    t[0] = 'R'; t[1] = 'R';   }

                printf("[%d, %c, %s]\t", turnMatrix[i][j].weight, d, t);
            }
        }
        printf("\n");
    }
    printf("\n");
}


