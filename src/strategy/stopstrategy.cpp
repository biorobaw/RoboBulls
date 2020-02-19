#include <iostream>
#include <list>

#include "utilities/measurements.h"
#include "behavior/goalie.h"
#include "behavior/refstop.h"
#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "stopstrategy.h"

StopStrategy::StopStrategy(Team* _team) : Strategy(_team)
{
}

void StopStrategy::assignBeh()
{
    for(Robot* robot: team->getRobots())
        robot->assignBeh<RefStop>();
}

bool StopStrategy::update()
{
    return false;
}
