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
    uint16_t node_count = 5;
    Edge edges[7];
    // A B C D E
    // 0 1 2 3 4
    //
    // A -6- B -5- C
    // |    /|    /
    // 1  2  2  5
    // |/    |/
    // C -1- D
    //
    edges[0].Init(0, 1, 6);
    edges[1].Init(0, 3, 1);
    edges[2].Init(1, 2, 5);
    edges[3].Init(1, 3, 2);
    edges[4].Init(1, 4, 2);
    edges[5].Init(2, 4, 5);
    edges[6].Init(3, 4, 1);

    navi.InitMap(edges, node_count);
    navi.PrintfGraph(5);

    navi.SetSection(MAZE_SECTION::s0pos);
    navi.GetNextMove(2);
    navi.GetNextMove(2);
    navi.GetNextMove(2);

    navi.GetNextMove(0);
    navi.GetNextMove(0);
    navi.GetNextMove(0);

    return 0;
}
