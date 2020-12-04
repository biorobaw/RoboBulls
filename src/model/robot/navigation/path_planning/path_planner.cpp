#include "path_planner.h"

#include "model/robot/robot.h"
#include "model/game_state.h"
#include "model/ball.h"

PathPlanner::PathPlanner(Robot* robot) :
    self(robot), game_state(robot->getGameState()), ball(game_state->getBall())
{

}

PathPlanner::~PathPlanner(){

}
