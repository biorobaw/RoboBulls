#include "haltstrategy.h"

#include "model/team.h"

HaltStrategy::HaltStrategy(RobotTeam* _team) : Strategy(_team) {

}

void HaltStrategy::assignBehaviors()
{
    for(Robot* robot : team->getRobots())
    {
        robot->setTargetVelocity(Point(0,0),0);

    }
}


std::string HaltStrategy::getName(){
    return "Halt";
}

