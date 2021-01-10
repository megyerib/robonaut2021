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
    MAZE_MOVE move;

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

    navi.PrintTrunMatrix(node_count);
    navi.InitMap(node_count);
    navi.PrintfGraph(node_count);

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
