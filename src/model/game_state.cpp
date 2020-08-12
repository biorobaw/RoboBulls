#include <sstream>
#include <iostream>
#include <fstream>

#include "strategy/strategycontroller.h"
#include "utilities/comparisons.h"

#include "utilities/edges.h"
#include "utilities/debug.h"
#include "gui/gui_interface.h"
#include "model/game_state.h"
#include "model/ball.h"
#include "model/team.h"
#include <assert.h>
#include "ssl_referee.pb.h"
#include "ssl/ssl_vision_listener.h"
#include "ssl/ssl_game_controller_listener.h"
#include "robot/robot.h"
#include "ball.h"

using std::cout, std::endl;

// ======= CONSTRUCTOR AND DATA UPDATER =====================

GameState::GameState(){
    referee_command  = Referee_Command_HALT; //The current state of the game from RefComm
    referee_command_previous  = Referee_Command_HALT;

    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            robots[i][j] = new Robot(i,j);
    ball = new Ball();

}

GameState::~GameState(){
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            delete robots[i][j];
    delete ball;
}

void GameState::update(){
    SSLVisionListener::copyState(this);
    SSLGameControllerListener::copyState(this);

    // update robots in field:
    for(int i=0; i< 2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++){
            auto r = robots[i][j];
            if(r->isInField()) {
                all_robots_in_field.insert(r);
                team_robots_in_field[i].insert(r);
            } else {
                all_robots_in_field.remove(r);
                team_robots_in_field[i].remove(r);
            }
        }

    // update robot who has the ball
    setRobotWithBall();

}

// ================ GETTER FUNCTIONS ========================

QSet<Robot*>& GameState::getFieldRobots(int team_id){
    return team_robots_in_field[team_id];
}
QSet<Robot*>& GameState::getFieldRobots(){
    return all_robots_in_field;
}

Robot* GameState::getRobot(int team_id, int robot_id){
    return robots[team_id][robot_id];
}
Robot* GameState::getBlueRobot(int id){
    return robots[ROBOT_TEAM_BLUE][id];
}
Robot* GameState::getYellowRobot(int id){
    return robots[ROBOT_TEAM_YELLOW][id];
}
Ball* GameState::getBall(){
    return ball;
}

Robot* GameState::getRobotWithBall(){
    return robot_with_ball;
}

Referee_Command GameState::getRefereeCommand(){
    return referee_command;
}
Referee_Command GameState::getRefereePreviousCommand(){
    return referee_command_previous;
}

char GameState::getGoals(int team_id){
    return goals[team_id];
}
int  GameState::getRemainingTime(){
    return remaining_time;
}


void GameState::setRobotWithBall(){
        // TODO: the function returns first match, not best match
        //Count of how many times `robotWithBall` has been seen without ball
        static int lastSeenWithoutBallCount = 0;

        // recompute which robot has the ball if no robot was in pocesion of the ball last cycle
        // or the previous holder has not been seen with the ball for 10 cycles
        if(!robot_with_ball || (!hasBall(robot_with_ball) && ++lastSeenWithoutBallCount > 10))
        {
            lastSeenWithoutBallCount = 0;
            robot_with_ball = nullptr;
            for(Robot* r : all_robots_in_field){
                if(hasBall(r)){
                    robot_with_ball = r;
                    return;
                }
            }
        }


}

bool GameState::hasBall(Robot* robot)
{
    if(!robot) return false;
    return Comparisons::isDistanceToLess(robot, ball->getPosition(), 300) &&
           Comparisons::isFacingPoint(robot, ball->getPosition());
}

void GameState::clearRefereeCommandChanged(){
    referee_command_changed = false;
}

bool GameState::hasRefereeCommandChanged(){
    return referee_command_changed;
}

