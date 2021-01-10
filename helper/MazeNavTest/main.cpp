#include <cstdio>
#include "MazeNavTest.h"
#include "TestCases.h"

// Units under test
#include "MazeNav_AlwaysLeft.h"
#include "mapnavigation.h"

int main()
{
    MazeNav_AlwaysLeft uut;

    size_t passed = 0;

    for (size_t i = 0; i < TEST_CASE_NUM; i++)
    {
        bool success = MazeNavTest::Test(uut, testCases[i]);

        passed += success ? 1 : 0;
        printf("Test case %zu: %s\n", i, success ? "PASS" : "FAIL");
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("%zu test cases passed out of %zu (%zu%%)\n\n", passed, TEST_CASE_NUM, passed*100/TEST_CASE_NUM);

    MapNavigation navi;
    uint16_t node_count = 12;
    uint16_t edge_count = 24;
    EDGE edges[edge_count];
    MAZE_MOVE move;

    //          <5
    //   -----x---x-----
    //  |    /  6  \    |
    // 1|2  3|4   7|8  9|10
    //  |   /  <11  \   |
    //   --x---------x--
    //          12
    edges[0].Init(0,  2, 11);
    edges[1].Init(0,  3, 11);
    edges[2].Init(0,  4,  8);
    edges[3].Init(0, 11, 12);
    edges[4].Init(1,  2, 11);
    edges[5].Init(1,  3, 11);
    edges[6].Init(1,  5,  8);
    edges[7].Init(1, 10, 12);
    edges[8].Init(2,  4,  3);
    edges[9].Init(2, 10,  9);
    edges[10].Init(3,  5,  3);
    edges[11].Init(3, 11,  9);
    edges[12].Init(4,  7,  4);
    edges[13].Init(4,  9,  9);
    edges[14].Init(5,  6,  4);
    edges[15].Init(5,  8,  9);
    edges[16].Init(6,  8, 13);
    edges[17].Init(6,  9,  9);
    edges[18].Init(6, 11,  9);
    edges[19].Init(7,  8,  9);
    edges[20].Init(7,  9, 13);
    edges[21].Init(7, 10,  9);
    edges[22].Init(8, 10, 12);
    edges[23].Init(9, 11, 12);

    navi.InitMap(edges, node_count, edge_count);
    navi.PrintfGraph(12);

    navi.SetSection(MAZE_SECTION::s0pos);
    move = navi.GetNextMove(2);
    move = navi.GetNextMove(2);
    move = navi.GetNextMove(2);

    move = navi.GetNextMove(0);
    move = navi.GetNextMove(0);
    move = navi.GetNextMove(0);

    //         <5
    //   -----B---C-----
    //  |    /  6  \    |
    //  |    |     |    |
    // 1|2  3|4   7|8  9|10
    //  |    |     |    |
    //  |   /  <11  \   |
    //   --A---------D--
    //          12

    // A
    TURN_INFO turnA0_1   = {  0,  1,  7, EXIT_DIR::edFrontMid};
    TURN_INFO turnA2_3   = {  2,  3,  4, EXIT_DIR::edRearRight};
    TURN_INFO turnA10_11 = { 10, 11,  5, EXIT_DIR::edRearLeft};
    navi.RegisterTurns(turnA0_1,   turnA2_3, TURN_POSITION::eMiddle, TURN_POSITION::eRight);
    navi.RegisterTurns(turnA0_1, turnA10_11, TURN_POSITION::eMiddle, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnA2_3, turnA10_11, TURN_POSITION::eRight,  TURN_POSITION::eLeft);

    // B
    TURN_INFO turnB1_0 = { 1, 0, 8, EXIT_DIR::edFrontRight};
    TURN_INFO turnB3_2 = { 3, 2, 3, EXIT_DIR::edFrontLeft};
    TURN_INFO turnB4_5 = { 4, 5, 0, EXIT_DIR::edRearMid};
    navi.RegisterTurns(turnB1_0, turnB3_2, TURN_POSITION::eRight, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnB1_0, turnB4_5, TURN_POSITION::eRight, TURN_POSITION::eMiddle);
    navi.RegisterTurns(turnB3_2, turnB4_5, TURN_POSITION::eLeft,  TURN_POSITION::eMiddle);

    // C
    TURN_INFO turnC5_4 = { 5, 4, 0, EXIT_DIR::edFrontMid};
    TURN_INFO turnC7_6 = { 7, 6, 4, EXIT_DIR::edRearLeft};
    TURN_INFO turnC9_8 = { 9, 8, 9, EXIT_DIR::edRearRight};
    navi.RegisterTurns(turnC5_4, turnC7_6, TURN_POSITION::eMiddle, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnC5_4, turnC9_8, TURN_POSITION::eMiddle, TURN_POSITION::eRight);
    navi.RegisterTurns(turnC7_6, turnC9_8, TURN_POSITION::eLeft,   TURN_POSITION::eRight);

    // D
    TURN_INFO turnD8_9   = {  8, 9, 6, EXIT_DIR::edFrontMid};
    TURN_INFO turnD6_7   = {  6,  7, 3, EXIT_DIR::edRearLeft};
    TURN_INFO turnD11_10 = { 11, 10, 6, EXIT_DIR::edRearRight};
    navi.RegisterTurns(turnD8_9,   turnD6_7, TURN_POSITION::eMiddle, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnD8_9, turnD11_10, TURN_POSITION::eMiddle, TURN_POSITION::eRight);
    navi.RegisterTurns(turnD6_7, turnD11_10, TURN_POSITION::eLeft,   TURN_POSITION::eRight);

    navi.PrintTrunMatrix(12);
    navi.InitMap(12);
    navi.PrintfGraph(12);

    navi.SetSection(MAZE_SECTION::s0pos);
    move = navi.GetNextMove(9);
    move = navi.GetNextMove(9);
    move = navi.GetNextMove(9);

    move = navi.GetNextMove(7);
    move = navi.GetNextMove(7);

    move = navi.GetNextMove(1);
    move = navi.GetNextMove(1);
    move = navi.GetNextMove(1);

    return 0;
}
