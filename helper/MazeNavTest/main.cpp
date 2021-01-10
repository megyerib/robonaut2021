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
    TURN_INFO turnA1_2   = {  1,  2,  7, EXIT_DIR::edFrontMid};
    TURN_INFO turnA3_4   = {  3,  4,  4, EXIT_DIR::edRearRight};
    TURN_INFO turnA11_12 = { 11, 12,  5, EXIT_DIR::edRearLeft};
    navi.RegisterTurns(turnA1_2,   turnA3_4, TURN_POSITION::eMiddle, TURN_POSITION::eRight);
    navi.RegisterTurns(turnA1_2, turnA11_12, TURN_POSITION::eMiddle, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnA3_4, turnA11_12, TURN_POSITION::eRight,  TURN_POSITION::eLeft);

    // B
    TURN_INFO turnB2_1 = { 2, 1, 8, EXIT_DIR::edFrontRight};
    TURN_INFO turnB4_3 = { 4, 3, 3, EXIT_DIR::edFrontLeft};
    TURN_INFO turnB5_6 = { 5, 6, 0, EXIT_DIR::edRearMid};
    navi.RegisterTurns(turnB2_1, turnB4_3, TURN_POSITION::eRight, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnB2_1, turnB5_6, TURN_POSITION::eRight, TURN_POSITION::eMiddle);
    navi.RegisterTurns(turnB4_3, turnB5_6, TURN_POSITION::eLeft,  TURN_POSITION::eMiddle);

    // C
    TURN_INFO turnC6_5  = {  6,  5, 0, EXIT_DIR::edFrontMid};
    TURN_INFO turnC8_7  = {  8,  7, 4, EXIT_DIR::edRearLeft};
    TURN_INFO turnC10_9 = { 10,  9, 9, EXIT_DIR::edRearRight};
    navi.RegisterTurns(turnC6_5,  turnC8_7, TURN_POSITION::eMiddle, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnC6_5, turnC10_9, TURN_POSITION::eMiddle, TURN_POSITION::eRight);
    navi.RegisterTurns(turnC8_7, turnC10_9, TURN_POSITION::eLeft,   TURN_POSITION::eRight);

    // D
    TURN_INFO turnD9_10  = {  9, 10, 6, EXIT_DIR::edFrontMid};
    TURN_INFO turnD7_8   = {  7,  8, 3, EXIT_DIR::edRearLeft};
    TURN_INFO turnD12_11 = { 12, 11, 6, EXIT_DIR::edRearRight};
    navi.RegisterTurns(turnD9_10,   turnD7_8, TURN_POSITION::eMiddle, TURN_POSITION::eLeft);
    navi.RegisterTurns(turnD9_10, turnD12_11, TURN_POSITION::eMiddle, TURN_POSITION::eRight);
    navi.RegisterTurns(turnD7_8,  turnD12_11, TURN_POSITION::eLeft,   TURN_POSITION::eRight);

    navi.PrintTrunMatrix(13);

    return 0;
}
