#include <iostream>
#include <list>

#include "utilities/measurements.h"
#include "../behaviors/goalie.h"
#include "../behaviors/refstop.h"
#include "model/game_state.h"
#include "utilities/comparisons.h"
#include "stopstrategy.h"
#include "model/team.h"
#include "robot/robot.h"

StopStrategy::StopStrategy(RobotTeam* _team) : Strategy(_team)
{
}

void StopStrategy::assignBehaviors()
{
    for(Robot* robot: team->getRobots())
        robot->assignBeh<RefStop>();
}

