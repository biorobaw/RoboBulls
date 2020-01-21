#include <iostream>
#include <list>

#include "utilities/measurements.h"
#include "behavior/goalie.h"
#include "behavior/refstop.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "stopstrategy.h"

StopStrategy::StopStrategy()
{
}

void StopStrategy::assignBeh()
{
    for(Robot* robot: gameModel->getMyTeam().getRobots())
        robot->assignBeh<RefStop>();
}

bool StopStrategy::update()
{
    return false;
}
