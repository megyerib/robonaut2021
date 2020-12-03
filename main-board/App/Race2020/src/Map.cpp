#include "Map.h"

#include "Trace.h"

Map* Map::GetInstance()
{
    static Map instance;
    return &instance;
}

void Map::Process()
{
    if (active == true)
    {
        prevTrackType   = actualTrackType;
        actualTrackType = trackDetect->GetTrackType();
        IsCarOverRoadsign();

        timeElapsed = UPTIME_us() - timeOrigo;

        actualPosition = navigation->GetPosition();
        actualDistance = encoder.GetDistance();

        switch (state)
        {
            case MapState::Drive:
            {
                decisionMade = false;

                if (overRoadsign == true)
                {
                    if ((IsCrosspoint() == true)                    ||
                        (actualTrackType == TrackType::Exit)        ||
                        (actualTrackType == TrackType::ExitReverse))
                    {
                        state = MapState::Decision;
                        PRINTF("__MAP: Decision");
                    }
                    else
                    {
                        state = MapState::OverRoadSign;
                        PRINTF("__MAP: OverRoadSign");
                    }
                }
                break;
            }
            case MapState::OverRoadSign:
            {
                if (IsCrosspoint() == true){        state = MapState::Decision;     PRINTF("__MAP: Decision");     }
                else if (overRoadsign == false){    state = MapState::Drive;        PRINTF("__MAP: Drive");     decisionMade = false; }
                else {}
                break;
            }
            case MapState::Decision:
            {
                DecideNextTurn();
                state = MapState::OverRoadSign;
                PRINTF("__MAP: OverRoadSign");
                decisionMade = true;
                break;
            }
            default:
                break;
        }
    }
}

TurnType Map::WhichWayToTurn()
{
    return nextTurn;
}

bool Map::isDecisionMade()
{
    return decisionMade;
}

bool Map::shouldExitMaze()
{
    return shouldExit;
}

MapState Map::GetState()
{
    return state;
}


void Map::TurnOn()
{
    active = true;
}

void Map::TurnOff()
{
    active = false;
}


Map::Map() : encoder(Encoder::GetInstance())
{
    trackDetect             = TrackDetector::GetInstance();
    navigation              = Navigation::GetInstance();

    active                  = true;

    segments[32]            = {0U};
    foundSegmentCount       = 0U;
    discovSegmentCount      = 0U;
    exitSegmentId           = INVALID_SEGMENT_ID;
    adjacents_AtoB[32][3]   = {INVALID_SEGMENT_ID};
    adjacents_BtoA[32][3]   = {INVALID_SEGMENT_ID};
    route[32]               = {TurnType::NotSet};
    direction               = CarDirection::AtoB;
    shouldExit              = false;

    state                   = MapState::Drive;
    remainingTime           = TIME_START;
    nextTurn                = TurnType::NoTurn;
    decisionMade            = false;
    overRoadsign            = false;

    actualTrackType         = TrackType::Single;
    prevTrackType           = TrackType::Single;
    timeElapsed             = 0;
    timeOrigo               = UPTIME_us();
    actualPosition          = navigation->GetPosition();
    actualDistance          = encoder.GetDistance();

    magic                   = 0;
}

void Map::DecideNextTurn()
{
    if (RANDOM_STRATEGY_ON == true)
    {
        // TODO Labyrinth algorithm
        // Add new segments
        // Update already know segments
        // Position can be set here if the end of the segment was found previously
        // Set adjacent segments
        // Plan route or discover new segment
    }
    else
    {
        nextTurn = RollDiceOnTurn();

        if (timeElapsed > TIME_GIVE_UP)       // TODO for exit maneuver
        {
            shouldExit = true;
        }
    }
}

TurnType Map::RollDiceOnTurn()
{
    uint8_t  randomNumber;
    TurnType turn;

    turn = TurnType::NoTurn;

    randomNumber = ( ((int)(encoder.GetDistance() * 10000) + (UPTIME_us()) ) % 255); // TODO no good (add random)
    if (randomNumber <= 85)
    {
        turn = TurnType::Left;
        PRINTF("__MAP: Left");
    }
    else if (randomNumber <= 170)
    {
        turn = TurnType::Right;
        PRINTF("__MAP: Right");
    }
    else
    {
        turn = TurnType::NoTurn;
        PRINTF("__MAP: NoTurn");
    }

    // Magic
    /*if (magic <= 15)
    {
        turn = TurnType::Right;
        PRINTF("__MAP: Force Right");
        magic++;
    }*/

    return turn;
}

void Map::IsCarOverRoadsign()
{
    if (actualTrackType == TrackType::Single)
    {
        overRoadsign = false;
    }
    else
    {
        if ((prevTrackType == TrackType::Single)
             && ((actualTrackType != TrackType::Single) /*|| (actualDistance != TrackType::None)*/))
        {
            overRoadsign = true;
        }
    }
}

bool Map::IsCrosspoint()
{
    bool crosspointFound = false;

    if ((trackDetect->IsJunction(prevTrackType) == true)
        &&
        ((trackDetect->IsFork(actualTrackType) == true) || (actualTrackType == TrackType::Single) == true))
    {
        crosspointFound = true;
        PRINTF("__MAP: CrossPoint");
    }

    if (prevTrackType == TrackType::Single
        &&
        (trackDetect->IsFork(actualTrackType) == true))
    {
        crosspointFound = true;
        PRINTF("__MAP: CrossPoint");
    }

    // The crossing point is the middle of the junction. This is the position registered to the segment.
    return crosspointFound;
}
