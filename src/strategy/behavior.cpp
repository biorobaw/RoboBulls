#include "behavior.h"
#include "robot/robot.h"

Behavior::Behavior(Robot* robot) :
    robot(robot), team(robot->getTeam()), game_state(robot->getGameState())
{

}

Behavior::~Behavior()
{

}

bool Behavior::isFinished()
{
    return true;
}
