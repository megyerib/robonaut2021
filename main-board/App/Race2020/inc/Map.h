#pragma once

#include "Encoder.h"
#include "TrackDetector.h"
#include "Uptime.h"

#include "Navigation.h"

#define MAP_SAMPLING_PERIOD    10 /* ms */      /* TODO check tasks */

#define RANDOM_STRATEGY_ON      (          0U)    /* 0 = use route planning, 1 = turn randomly. */
#define EPSILON                 (      0.001f)    /* Compare margin for floats. */
#define TIME_LIMIT              (  190000000U)    /* Max time that can be spent in the labyrinth in us. */
#define TIME_START              (   20000000U)    /* Time available at the start in us. */
#define TIME_SEGM_FOUND         (   10000000U)    /* Tied for adjacent not found yet in us. */
#define TIME_GIVE_UP            (  100000000U)    /* In random mode search for the exit after this time */
#define INVALID_SEGMENT_ID      (        255U)    /* Segment ID used for not discovered adjacent and initial value. */

typedef enum
{
    BtoA = 0,
    AtoB = 1
} CarDirection;

typedef enum
{
    NotSet = 0,
    Left,
    Right,
    NoTurn,
    Reverse,
    ExitRight,
    ExitLeft,
    Invalid = 255
} TurnType;

typedef struct
{
    uint8_t      id;
    uint16_t     len;
    CarDirection dir;
    Position     pos_A;
    Position     pos_B;
    TurnType     turn_A;
    TurnType     turn_B;
    bool         discovered;
} Segment;

typedef enum
{
    Drive = 0,
    OverRoadSign,
    Decision
} MapState;

class Map
{
private:
    Navigation*    navigation;
    Encoder&       encoder;
    TrackDetector* trackDetect;

    bool         active;

    Segment      segments[32];
    uint8_t      foundSegmentCount;
    uint8_t      discovSegmentCount;
    uint8_t      exitSegmentId;
    uint8_t      adjacents_AtoB[32][3];
    uint8_t      adjacents_BtoA[32][3];
    TurnType     route[32];
    CarDirection direction;
    bool         shouldExit;

    MapState     state;
    uint32_t     remainingTime;
    TurnType     nextTurn;
    bool         decisionMade;
    bool         overRoadsign;

    TrackType    actualTrackType;
    TrackType    prevTrackType;
    uint32_t     timeElapsed;
    uint32_t     timeOrigo;
    Position     actualPosition;
    uint32_t     actualDistance;

    uint8_t      magic;

public:
    static Map* GetInstance();

    void Process();
    TurnType WhichWayToTurn();
    bool isDecisionMade();
    bool IsCrosspoint();
    bool shouldExitMaze();
    MapState GetState();

    void TurnOn();
    void TurnOff();

private:

    Map();

    void DecideNextTurn();

    TurnType RollDiceOnTurn();
    void IsCarOverRoadsign();


    bool cmpf(float const a, float const b, float const epsilon)
    {
       return (float(fabs(double(a - b))) < epsilon);
    }
};
