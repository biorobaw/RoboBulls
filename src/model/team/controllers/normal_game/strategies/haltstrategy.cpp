#include "haltstrategy.h"

#include "model/team/team.h"

HaltStrategy::HaltStrategy(RobotTeam* _team) : TeamStrategy(_team) {

}

void HaltStrategy::assignBehaviors()
{
    for(Robot* robot : team->getRobots())
    {
        robot->setTargetVelocityLocal(Point(0,0),0);

    }
}


QString HaltStrategy::getName(){
    return "Halt";
}

