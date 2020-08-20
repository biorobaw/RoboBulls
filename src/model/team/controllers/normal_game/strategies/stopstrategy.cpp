#include <iostream>
#include <list>

#include "stopstrategy.h"
#include "model/team/team.h"
#include "model/team/behaviors/refstop.h"


StopStrategy::StopStrategy(RobotTeam* _team) : TeamStrategy(_team)
{
}

void StopStrategy::assignBehaviors()
{
    for(Robot* robot: team->getRobots())
        robot->assignBeh<RefStop>();
}


QString StopStrategy::getName(){
    return "Stop";
}
