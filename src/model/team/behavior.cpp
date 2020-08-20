#include "behavior.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

Behavior::Behavior(Robot* robot) :
    game_state(robot->getGameState()), team(robot->getTeam()), robot(robot), ball(robot->getGameState()->getBall())
{

}

Behavior::~Behavior()
{

}

bool Behavior::isFinished()
{
    return true;
}
