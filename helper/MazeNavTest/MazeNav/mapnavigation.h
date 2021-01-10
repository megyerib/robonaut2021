#pragma once

// Abstract Interfaces.
#include "MazeNavBase.h"

// Types.
#include "map_types.h"

// Used modules.
#include "dijkstra.h"

// Own Types.
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

typedef MOVE_INFO TURN_MATRIX[MAX_VERTEX][MAX_VERTEX];

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

// ON = 1U, OFF = 0U
#define DEBUG_NAVI_FUNC_ON   (0U)

class MapNavigation : public MazeNavBase
{
public:
    MapNavigation();

    virtual MAZE_MOVE GetNextMove(uint8_t target) override;
    virtual void SetSection(MAZE_SECTION section) override;

    void InitMap(uint16_t const node_count);
    void AddJunction(TRUNTABLE const junction);
    void RegisterTurns(TURN_INFO const from,
                       TURN_INFO const to,
                       TURN_POSITION const tpos_from,
                       TURN_POSITION const tpos_to);

private:
    // Map stored as a graph.
    uint16_t    vertex_count;
    TURN_MATRIX turnMatrix;
    VERTEX      actual_vertex;

    // Shortest path properties.
    Dijkstra        pathfinder_algorithm;
    DIJKSTRA_RESULT pathfinder_result;
    VERTEX          source_vertex;
    VERTEX          target_vertex;
    PATH            shortest_path;
    uint8_t         step_count;

    void PlanRoute();

    VERTEX GetNextVertex();

    bool IsTurnInfoValid(TURN_INFO const turn_info);

#if DEBUG_NAVI_FUNC_ON == 1U
    void PrintTrunMatrix(int size);
    void PrintPathMoves(int size);
#endif

    template<typename T>
    void InitArray(T* array, T value, size_t size)
    {
        for (size_t i = 0; i < size; i++)
        {
            array[i] = value;
        }
    }
};

