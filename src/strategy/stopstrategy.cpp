#include <iostream>
#include <list>

#include "src/utilities/measurements.h"
#include "src/behavior/goalie.h"
#include "src/behavior/refstop.h"
#include "src/model/game_state.h"
#include "src/utilities/comparisons.h"
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
