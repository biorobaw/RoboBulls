#include <sstream>
#include <iostream>
#include <fstream>

#include "strategy/strategycontroller.h"
#include "utilities/comparisons.h"

#include "utilities/edges.h"
#include "utilities/debug.h"
#include "gui/guiinterface.h"
#include "model/game_state.h"
#include "model/ball.h"
#include <assert.h>


std::mutex GameState::my_team_mutex;
std::mutex GameState::opp_team_mutex;

// Global static pointer used to ensure a single instance of the class.
GameState* gameState = new GameState();

/*******************************************************************/
/************************ Public Methods ***************************/
/*******************************************************************/


//! @brief From the RefComm, Returns the number of Blue goals
char GameState::getBlueGoals()
{
    return blueGoals;
}

//! @brief From the RefComm, Returns the number of Yellow goals
char GameState::getYellowGoals()
{
    return yellowGoals;
}

//! @brief Returns the remaining time in seconds
short GameState::getRemainingTime()
{
    return remainingTime;
}

//! @brief Returns the current game state, used by StrategyController
char GameState::getState()
{
    return state;
}


//! @brief Returns true id RefComm has sent a new command
bool GameState::isNewCommand()
{
    return this->hasNewCommand;
}

//! @brief Returns the penalty point that penalty kicks are taken from
Point GameState::getPenaltyPoint()
{
    return Point(2045, 22);
}

//! @brief Returns the opponents's goal, that we are trying to score in
//! VisionComm transforms received info such that opponent is always
//! on the positive side based on the SIDE global
Point GameState::getOppGoal()
{
    return Point(HALF_FIELD_LENGTH, 0);
}

//! @brief Returns the goal point that we are defending
//! //! VisionComm transforms received info such that we are always
//! on the negative side based on the SIDE global
Point GameState::getMyGoal()
{
    return Point(-HALF_FIELD_LENGTH, 0);
}

//! @brief Returns the last different game state before this one
char GameState::getPreviousState()
{
    return previousState;
}

//! @brief Returns a string representation of the GameModel, including all robots and the ball
std::string GameState::toString()
{
    std::stringstream myString;

    myString << "Ball Position: " << Ball::getPosition().toString() << std::endl;

    myString<<"\nBlue Team Robots: \n";
    auto robots = Team::getTeam(TEAM_BLUE)->getRobots();
    for (auto it = robots.begin(); it != robots.end(); it++)
    {
        myString << "\t" << (*it)->toString()<< std::endl;
    }

    myString<<"\nYellow Team Robots: \n";
    robots = Team::getTeam(TEAM_YELLOW)->getRobots();
    for(auto it = robots.begin(); it != robots.end(); it++)
    {
        myString << "\t" << (*it)->toString() << std::endl;
    }

    return myString.str();
}

/*! @brief Queues a robot replacement for robot with an id on a team.
 * @details [grSim_Replacement](https://github.com/roboime/ssl-sim/blob/master/protos/grSim_Replacement.proto)
 * allows for position of angle changes of simulated robots using packets. This function asks the simulator
 * to move a robot to an x and y point.
 * @param id ID of robot to move
 * @param team TEAM_BLUE or TEAM_YELLOW
 * @param x X Position to move robot to
 * @param y Y Positon to move robot to
 * @param dir Orientation to set robot at (leave blank to keep current robot orientation) */
void GameState::addRobotReplacement(int id, int team, float x, float y, float dir)
{
    //Keep orientation if left blank
    if(dir == -10) {
        Robot* robot = Team::getTeam(team)->getRobot(id);
        dir = robot->getOrientation();
    }
    RobotReplacement replacement {id, team, x, y, dir};
    robotReplacements.push_back(replacement);
    hasRobotReplacements = true;
}

/*! @brief Creates a ball replacement request
 * @details Like addRobotReplacement, grSim allows us to change the position of the ball with replacement packets.
 * @param x X Position to move ball to
 * @param y Y Positon to move ball to
 * @param vx X Velocity in m/s to set the ball to
 * @param vx Y Velocity in m/s to set the ball to */
void GameState::addBallReplacement(float x, float y, float vx, float vy)
{
    ballReplacement = {x, y, vx, vy};
    hasBallReplacement = true;
}

/*******************************************************************/
/************************ Private Methods **************************/
/*******************************************************************/

/* Called by VisionComm */
/* Don't overlook this function, it's more important than you think */
/*! @brief Used by VisionComm; The main shebang; calls the run functions of the
 * StrategyController and makes the project work. This also updates the GUI system. */
void GameState::notifyObservers()
{
    //std::cout << "at GameModel::notifyObservers()\n";
    Ball::setRobotWithBall();
    Team* t = Team::getTeam(TEAM_BLUE);
    if(t->isControlled()) t->controller.run();
    t = Team::getTeam(TEAM_YELLOW);
    if(t->isControlled()) t->controller.run();
}

/*! @brief Used by RefComm; Set the current game state (See RefComm reference)
 * @see RefComm
 * @see getGameState */
void GameState::setGameState(char gameState)
{
    char lastGameState = this->state;
    if(lastGameState != gameState)
    {
        hasNewCommand = true;
        previousState = lastGameState;
    }
    this->state = gameState;
}

//! @brief Callback called from StrategyController when a new command is processed
void GameState::onCommandProcessed()
{
    this->hasNewCommand = false;
}


//! @brief Used by RefComm; Set the current game state (See RefComm reference)
void GameState::setTimeLeft(short time)
{
    remainingTime = time;
}

//! @brief Used by RefComm; Set the number of blue team goals
void GameState::setBlueGoals(char goals)
{
    blueGoals = goals;
}

//! @brief Used by RefComm; Set the number of yellow team goals
void GameState::setYellowGoals(char goals)
{
    yellowGoals = goals;
}














