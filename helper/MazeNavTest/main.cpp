#include <cstdio>
#include "MazeNavTest.h"
#include "TestCases.h"
#include "mapgenerator.h"

// Units under test
#include "MazeNav_AlwaysLeft.h"
#include "mapnavigation.h"


int main()
{
    MazeNav_AlwaysLeft uut;
    MapNavigation navi;

    size_t passed = 0;

    for (size_t i = 0; i < TEST_CASE_NUM; i++)
    {
        bool success = MazeNavTest::Test(navi, testCases[i]);

        passed += success ? 1 : 0;
        printf("Test case %zu: %s\n\n", i, success ? "PASS" : "FAIL");
    }

    printf("--------------------------------------------------------------------------------\n");
    printf("%zu/%zu test cases passed (%zu%%)\n\n", passed, TEST_CASE_NUM, passed*100/TEST_CASE_NUM);

// HOW-TO_____
// New map generator.
//    1. USE_STATIC_MAP shall be 0 and MAP_GENERATOR_ACTIVE shall be 1 (config defines).
//    2. You can adjust the weigth calculation with BACKWARD_PENALTY and DIRECTION_CHANGE_PENALTY.
//    3. The map will be generated next to the .exe file as 'static_map.txt'.
//    4. You HAVE TO copy the context of the generated file into the static_map.h
//    5. Set the vertex count in the static_map.h.
//
// The generation was successful and the map is now saved as the new static map. You can set back the
// config defines.
// Warning: THe unit tester code might cause error, so comment out during gmap update.
#if MAP_GENERATOR_ACTIVE == 1U
    MapGenerator generator;
    generator.RobonautTestMapGenerator();
#endif

    return 0;
}
