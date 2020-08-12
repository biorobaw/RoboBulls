#include <iostream>
#include <list>

#include "stopstrategy.h"
#include "model/team.h"
#include "strategy/behaviors/refstop.h"


StopStrategy::StopStrategy(RobotTeam* _team) : Strategy(_team)
{
}

void StopStrategy::assignBehaviors()
{
    for(Robot* robot: team->getRobots())
        robot->assignBeh<RefStop>();
}


std::string StopStrategy::getName(){
    return "Stop";
}
