#include "mapnavigation.h"
#include <cstring>
#include <cstdio>
#if USE_STATIC_MAP == 1U
#include "static_map.h"
#endif

MapNavigation::MapNavigation()
{
#if USE_STATIC_MAP == 0U
    vertex_count      = 0U;
    memset(turn_matrix, 0U, sizeof(turn_matrix));
#endif
    actual_vertex     = 0U;
    pathfinder_result.target = INVALID_VERTEX;
    InitArray<uint16_t>(pathfinder_result.distance_list, INF_DIST, sizeof(pathfinder_result.distance_list)/sizeof (pathfinder_result.distance_list[0]));
    InitArray<uint8_t>(pathfinder_result.prev_vertex_list, INVALID_VERTEX, sizeof(pathfinder_result.prev_vertex_list));
    source_vertex     = 0U;
    target_segment    = 0U;
    target_vertex_pos = 0U;
    target_vertex_neg = 0U;
    InitArray<uint8_t>(shortest_path, INVALID_VERTEX, sizeof(shortest_path));
    step_count        = 0U;


#if USE_STATIC_MAP == 1U
    InitMap();
#endif
}

MAZE_MOVE MapNavigation::GetNextMove(uint8_t target)
{
    MAZE_MOVE next_move;
    VERTEX next_vertex;
    const TURN_MATRIX& turnMatrix = GetTurnMatrix();

    if (target_segment != target)
    {
        target_segment    = target;
        target_vertex_pos = target_segment * 2U;
        target_vertex_neg = target_vertex_pos + 1U;
        source_vertex = actual_vertex;
        PlanRoute();
#if DEBUG_PATH_DISP == 1U
        PrintPathMoves(step_count);
#endif
    }

    next_vertex = GetNextVertex();

    next_move.apprDir = turnMatrix[actual_vertex][next_vertex].direction;
    next_move.exitDir = turnMatrix[actual_vertex][next_vertex].turning;

    if (actual_vertex == pathfinder_result.target)
    {
        next_move.arrived = true;
    }
    else
    {
        next_move.arrived = false;
    }

    actual_vertex = next_vertex;

    return next_move;
}

void MapNavigation::SetCurrentSection(MAZE_SECTION section)
{
    actual_vertex = section;
}

MAZE_SECTION MapNavigation::GetCurrentSection()
{
    return (MAZE_SECTION)actual_vertex;
}

#if USE_STATIC_MAP == 1U
void MapNavigation::InitMap()
{
    uint8_t vertexCount = GetVertexCount();
    const TURN_MATRIX& matrix = GetTurnMatrix();

    pathfinder_algorithm.InitGraph(vertexCount);

    for (int i = 0U; i < vertexCount; i++)
    {
        for (int j = 0U; j < vertexCount; j++)
        {
            pathfinder_algorithm.AddEdge(i, j, matrix[i][j].weight);
        }
    }

#if DEBUG_NAVI_FUNC_ON == 1U
    PrintTrunMatrix(vertexCount);
#endif
#if DEBUG_DIJKTSRA_FUNC_ON == 1U
    pathfinder_algorithm.PrintfGraph(vertexCount);
#endif
}
#else
void MapNavigation::InitMap(const uint16_t node_count)
{
    vertex_count = node_count;
    const TURN_MATRIX& matrix = GetTurnMatrix();

    pathfinder_algorithm.InitGraph(vertex_count);

    for (int i = 0U; i < vertex_count; i++)
    {
        for (int j = 0U; j < vertex_count; j++)
        {
            pathfinder_algorithm.AddEdge(i, j, matrix[i][j].weight);
        }
    }

#if DEBUG_NAVI_FUNC_ON == 1U
    PrintTrunMatrix(vertex_count);
#endif
#if DEBUG_DIJKTSRA_FUNC_ON == 1U
    pathfinder_algorithm.PrintfGraph(vertex_count);
#endif
}

void MapNavigation::AddJunction(const TRUNTABLE junction)
{
    (void)junction;
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
        turn_matrix[to.vertex_in][from.vertex_in].direction   = APPR_DIR::adForward;
        turn_matrix[to.vertex_out][from.vertex_out].direction = APPR_DIR::adBackward;

        turn_matrix[to.vertex_in][from.vertex_in].weight   = distance * DIRECTION_CHANGE_PENALTY;
        turn_matrix[to.vertex_out][from.vertex_out].weight = distance * DIRECTION_CHANGE_PENALTY * BACKWARD_PENALTY;

        switch (tpos_from)
        {
            case tpLeft:
            {
                if (isFromFront == true)
                {
                    turn_matrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turn_matrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontLeft;
                }
                else
                {
                    turn_matrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turn_matrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                break;
            }
            case tpMiddle:
            {
                turn_matrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearMid;
                turn_matrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontMid;
                break;
            }
            case tpRight:
            {
                if (isFromFront == true)
                {
                    turn_matrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turn_matrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                else
                {
                    turn_matrix[to.vertex_in][from.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turn_matrix[to.vertex_out][from.vertex_out].turning = EXIT_DIR::edFrontLeft;
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
        turn_matrix[from.vertex_in][to.vertex_in].direction   = APPR_DIR::adForward;
        turn_matrix[from.vertex_out][to.vertex_out].direction = APPR_DIR::adBackward;

        turn_matrix[from.vertex_in][to.vertex_in].weight   = distance * DIRECTION_CHANGE_PENALTY;
        turn_matrix[from.vertex_out][to.vertex_out].weight = distance * DIRECTION_CHANGE_PENALTY * BACKWARD_PENALTY;

        switch (tpos_to)
        {
            case tpLeft:
            {
                if (isFromFront == true)
                {
                    turn_matrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turn_matrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontLeft;
                }
                else
                {
                    turn_matrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turn_matrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                break;
            }
            case tpMiddle:
            {
                turn_matrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearMid;
                turn_matrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontMid;
                break;
            }
            case tpRight:
            {
                if (isFromFront == true)
                {
                    turn_matrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearLeft;
                    turn_matrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontRight;
                }
                else
                {
                    turn_matrix[from.vertex_in][to.vertex_in].turning   = EXIT_DIR::edRearRight;
                    turn_matrix[from.vertex_out][to.vertex_out].turning = EXIT_DIR::edFrontLeft;
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
        turn_matrix[to.vertex_in][from.vertex_out].direction = APPR_DIR::adForward;
        turn_matrix[to.vertex_out][from.vertex_in].direction = APPR_DIR::adBackward;

        turn_matrix[to.vertex_in][from.vertex_out].weight = distance;
        turn_matrix[to.vertex_out][from.vertex_in].weight = distance * BACKWARD_PENALTY;

        switch (tpos_from)
        {
            case tpLeft:
            {
                turn_matrix[to.vertex_in][from.vertex_out].turning = EXIT_DIR::edFrontLeft;
                turn_matrix[to.vertex_out][from.vertex_in].turning = EXIT_DIR::edRearRight;
                break;
            }
            case tpMiddle:
            {
                turn_matrix[to.vertex_in][from.vertex_out].turning = EXIT_DIR::edFrontMid;
                turn_matrix[to.vertex_out][from.vertex_in].turning = EXIT_DIR::edRearMid;
                break;
            }
            case tpRight:
            {
                turn_matrix[to.vertex_in][from.vertex_out].turning = EXIT_DIR::edFrontRight;
                turn_matrix[to.vertex_out][from.vertex_in].turning = EXIT_DIR::edRearLeft;
                break;
            }
            default:
            {
                // Error.
                break;
            }
        }

        // From -> To
        turn_matrix[from.vertex_in][to.vertex_out].direction = APPR_DIR::adForward;
        turn_matrix[from.vertex_out][to.vertex_in].direction = APPR_DIR::adBackward;

        turn_matrix[from.vertex_in][to.vertex_out].weight = distance;
        turn_matrix[from.vertex_out][to.vertex_in].weight = distance * BACKWARD_PENALTY;

        switch (tpos_to)
        {
            case tpLeft:
            {
                turn_matrix[from.vertex_in][to.vertex_out].turning = EXIT_DIR::edFrontRight;
                turn_matrix[from.vertex_out][to.vertex_in].turning = EXIT_DIR::edRearLeft;
                break;
            }
            case tpMiddle:
            {
                turn_matrix[from.vertex_in][to.vertex_out].turning = EXIT_DIR::edFrontMid;
                turn_matrix[from.vertex_out][to.vertex_in].turning = EXIT_DIR::edRearMid;
                break;
            }
            case tpRight:
            {
                turn_matrix[from.vertex_in][to.vertex_out].turning = EXIT_DIR::edFrontLeft;
                turn_matrix[from.vertex_out][to.vertex_in].turning = EXIT_DIR::edRearRight;
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
#endif

void MapNavigation::PlanRoute()
{
    uint8_t actual_vertex = INVALID_VERTEX;
    uint8_t vertexCount = GetVertexCount();

    pathfinder_algorithm.CalculatePath(source_vertex, target_vertex_pos, target_vertex_neg);
    pathfinder_result = pathfinder_algorithm.GetResult();

    shortest_path[0U] = pathfinder_result.target;
    actual_vertex     = pathfinder_result.target;

    step_count = 1U;
    while ((actual_vertex != source_vertex) && (step_count < vertexCount))
    {
        actual_vertex = pathfinder_result.prev_vertex_list[actual_vertex];
        shortest_path[step_count] = actual_vertex;
        step_count++;
    }
}

VERTEX MapNavigation::GetNextVertex()
{
    VERTEX next_vertex = INVALID_VERTEX;
    uint8_t index = 0U;

    while (index < step_count)
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


bool MapNavigation::IsTurnInfoValid(const TURN_INFO turn_info)
{
    bool isValid = false;

    if ((turn_info.vertex_in != INVALID_VERTEX) &&
        (turn_info.vertex_out != INVALID_VERTEX) &&
        (turn_info.weight > 0U))
    {
        isValid = true;
    }

    return isValid;
}

const TURN_MATRIX &MapNavigation::GetTurnMatrix()
{
#if USE_STATIC_MAP == 1U
    return cfgTurnMatrix;
#else
    return turn_matrix;
#endif    
}

uint8_t MapNavigation::GetVertexCount()
{
#if USE_STATIC_MAP == 1U
    return cfgVertexCount;
#else
    return vertex_count;
#endif
}

#if DEBUG_NAVI_FUNC_ON == 1U
void MapNavigation::PrintTrunMatrix(int size)
{
    const TURN_MATRIX& matrix = GetTurnMatrix();

    for (int i = 0U; i < size; i++)
    {
        for (int j = 0U; j < size; j++)
        {
            if (matrix[i][j].weight == 65535)
            {
                printf("[0, 0, 00]\t");
            }
            else
            {
                char d = '0';
                if (matrix[i][j].direction == APPR_DIR::adBackward){    d = 'B';    }
                else { d = 'F'; }

                char t[3] = "00";
                if (matrix[i][j].turning == EXIT_DIR::edFrontLeft){         t[0] = 'F'; t[1] = 'L';   }
                else if (matrix[i][j].turning == EXIT_DIR::edFrontMid){     t[0] = 'F'; t[1] = 'M';   }
                else if (matrix[i][j].turning == EXIT_DIR::edFrontRight){   t[0] = 'F'; t[1] = 'R';   }
                else if (matrix[i][j].turning == EXIT_DIR::edRearLeft){     t[0] = 'R'; t[1] = 'L';   }
                else if (matrix[i][j].turning == EXIT_DIR::edRearMid){      t[0] = 'R'; t[1] = 'M';   }
                else if (matrix[i][j].turning == EXIT_DIR::edRearRight){    t[0] = 'R'; t[1] = 'R';   }

                printf("[%d, %c, %s]\t", matrix[i][j].weight, d, t);
            }
        }
        printf("\n");
    }
    printf("\n");
}
#endif

#if DEBUG_PATH_DISP == 1U
void MapNavigation::PrintPathMoves(int size)
{
    VERTEX next = INVALID_VERTEX;
    VERTEX act  = INVALID_VERTEX;
    const TURN_MATRIX& turnMatrix = GetTurnMatrix();
    char d    = '0';
    char t[3] = "00";
    char a    = '0';

    for (uint8_t i = size-1U; i > 0U; i--)
    {
        act  = shortest_path[i];
        next = shortest_path[i-1U];

        if (turnMatrix[act][next].direction == APPR_DIR::adBackward){       d = 'B';    }
        else if (turnMatrix[act][next].direction == APPR_DIR::adForward){   d = 'F';    }
        else { d = '?'; }

        if (turnMatrix[act][next].turning == EXIT_DIR::edFrontLeft){         t[0] = 'F'; t[1] = 'L';   }
        else if (turnMatrix[act][next].turning == EXIT_DIR::edFrontMid){     t[0] = 'F'; t[1] = 'M';   }
        else if (turnMatrix[act][next].turning == EXIT_DIR::edFrontRight){   t[0] = 'F'; t[1] = 'R';   }
        else if (turnMatrix[act][next].turning == EXIT_DIR::edRearLeft){     t[0] = 'R'; t[1] = 'L';   }
        else if (turnMatrix[act][next].turning == EXIT_DIR::edRearMid){      t[0] = 'R'; t[1] = 'M';   }
        else if (turnMatrix[act][next].turning == EXIT_DIR::edRearRight){    t[0] = 'R'; t[1] = 'R';   }
        else{ t[0] = '?'; t[1] = '?'; }

        if (act == pathfinder_result.target){    a = 'T';    }
        else{   a = 'F';    }
        (void)a;

        printf("[%d, %c, %s]\t", turnMatrix[act][next].weight, d, t);
    }
    printf("\n");
}
#endif

