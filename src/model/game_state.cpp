#include <sstream>
#include <iostream>
#include <fstream>

#include "model/team/team_strategy_controller.h"
#include "utilities/comparisons.h"

#include "utilities/edges.h"
#include "utilities/debug.h"
#include "gui/interface/gui_interface.h"
#include "model/game_state.h"
#include "model/ball.h"
#include "model/team/team.h"
#include <assert.h>
#include "ssl_referee.pb.h"
#include "ssl/ssl_vision_listener.h"
#include "ssl/ssl_game_controller_listener.h"
#include "model/robot/robot.h"
#include "ball.h"

#include "gui/interface/data/gui_robot.h"

using std::cout, std::endl;

// ======= CONSTRUCTOR AND DATA UPDATER =====================

GameState::GameState(QObject* parent) :
    QObject(parent),
    Collisions(this),
    ball(new Ball),
    referee_command(Referee_Command_HALT),
    referee_command_previous(Referee_Command_HALT)
{
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++){
            robots[i][j] = new Robot(i,j);
            robots[i][j]->setGameState(this)
                        ->setParent(this);
        }

}

GameState::~GameState(){
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

    updateAllCollisions();

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
    // recompute which robot has the ball if no robot was in pocession
    // for at least 150ms to avoid false negatives

    if(robot_with_ball) if(hasBall(robot_with_ball)) last_time_with_ball = time_stamp;
    if(time_stamp - last_time_with_ball > 0.15) // 150ms
    {
        if(robot_with_ball) robot_with_ball->setHasBall(false);
        robot_with_ball = nullptr;
        for(Robot* r : all_robots_in_field){
            if(hasBall(r)){
                last_time_with_ball = time_stamp;
                robot_with_ball = r;
                r->setHasBall(true);
                return;
            }
        }
    }


}

bool GameState::hasBall(Robot* robot)
{
    if(!robot) return false;
    return Comparisons::isDistanceToLess(robot, *ball, 300) &&
           Comparisons::isFacingPoint(robot, *ball);
}

void GameState::clearRefereeCommandChanged(){
    referee_command_changed = false;
}

bool GameState::hasRefereeCommandChanged(){
    return referee_command_changed;
}

void GameState::setFlipXCoorinates(bool flip_x){
    ball->setFlipXCoordinates(flip_x);
    for(int i=0; i<2; i++)
        for(int j=0; j<MAX_ROBOTS_PER_TEAM; j++)
            robots[i][j]->setFlipXCoordinates(flip_x);
}




void GameState::refereeCommandChanged(int new_value, int old_value){
    referee_command_changed  = true;
    referee_command          = (Referee_Command)new_value;
    referee_command_previous = (Referee_Command)old_value;

}

void GameState::goalsChanged(int blue_goals, int yellow_goals){
    goals[0] = blue_goals;
    goals[1] = yellow_goals;

}

