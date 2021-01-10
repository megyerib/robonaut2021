#include "MazeNavTest.h"

bool MazeNavTest::Test(MazeNavBase& nav, NAV_TEST_CASE& testCase)
{
    nav.SetCurrentSection(testCase.startSection);

    for (size_t i = 0; i < testCase.stepNum; i++)
    {
        MAZE_MOVE  result = nav.GetNextMove(testCase.steps[i].target);
        MAZE_MOVE& expRes = testCase.steps[i].expRes;

        if (expRes.arrived == false)
        {
            if (result.apprDir != expRes.apprDir ||
                result.exitDir != expRes.exitDir ||
                result.arrived != false)
            {
                return false;
            }
        }
        else
        {
            if (result.arrived != true)
            {
                return false;
            }
        }
    }

    return true;
}
