#include <sstream>
#include <iostream>
#include <fstream>

#include "strategy/strategycontroller.h"
#include "utilities/comparisons.h"

#include "utilities/edges.h"
#include "utilities/debug.h"
#include "gui/guiinterface.h"
#include "model/gamemodel.h"
#include <assert.h>


std::mutex GameModel::my_team_mutex;
std::mutex GameModel::opp_team_mutex;

// Global static pointer used to ensure a single instance of the class.
GameModel* gameModel = new GameModel();

/*******************************************************************/
/************************ Public Methods ***************************/
/*******************************************************************/


/*! @brief Returns the robot that currently has the ball
 * A robot has the ball if it is close to it and is facing it */
Robot* GameModel::getHasBall()
{
    return robotWithBall;
}



/*! @brief Return a 2D point of the current ball location
 * \return A Point with the ball's position */
Point GameModel::getBallPoint()
{
    //std::cout<<"Ball point is at: "<<ballPoint.toString()<<std::endl;             //Added by Bo Wu for testing
    return ballPoint;
}

/*! @brief Return a 2D point of the X and Y component of the ball's velocity
 * \see Point
 * \see VelocityCalculator
 * \return The ball's velocity as an X/Y point in m/s*/
Point GameModel::getBallVelocity()
{
    return ballVelocity;
}

/*! @brief Return a predicted stopping point for the ball
 * \return A Point with the ball's position */
Point GameModel::getBallStopPoint()
{
    return ballStopPoint;
}

/*! @brief Returns the ball's <i>speed</i> (magnitude of velocity) in m/s */
float GameModel::getBallSpeed()
{
    return Measurements::mag(ballVelocity);
}


//! @brief From the RefComm, Returns the number of Blue goals
char GameModel::getBlueGoals()
{
    return blueGoals;
}

//! @brief From the RefComm, Returns the number of Yellow goals
char GameModel::getYellowGoals()
{
    return yellowGoals;
}

//! @brief Returns the remaining time in seconds
short GameModel::getRemainingTime()
{
    return remainingTime;
}

//! @brief Returns the current game state, used by StrategyController
char GameModel::getGameState()
{
    return gameState;
}

/*! @brief To get the singleton instance of the GameModel.
 *  @deprecated Use gameModel global pointer instead  */
GameModel * GameModel::getModel()
{
    return gameModel;
}

//! @brief Returns true id RefComm has sent a new command
bool GameModel::isNewCommand()
{
    return this->hasNewCommand;
}

//! @brief Returns the penalty point that penalty kicks are taken from
Point GameModel::getPenaltyPoint()
{
    return Point(2045, 22);
}

//! @brief Returns the opponents's goal, that we are trying to score in
//! VisionComm transforms received info such that opponent is always
//! on the positive side based on the SIDE global
Point GameModel::getOppGoal()
{
    return Point(HALF_FIELD_LENGTH, 0);
}

//! @brief Returns the goal point that we are defending
//! //! VisionComm transforms received info such that we are always
//! on the negative side based on the SIDE global
Point GameModel::getMyGoal()
{
    return Point(-HALF_FIELD_LENGTH, 0);
}

//! @brief Returns the last different game state before this one
char GameModel::getPreviousGameState()
{
    return previousGameState;
}

//! @brief Returns a string representation of the GameModel, including all robots and the ball
std::string GameModel::toString()
{
    std::stringstream myString;

    myString << "Ball Position: " << ballPoint.toString() << std::endl;

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
void GameModel::addRobotReplacement(int id, int team, float x, float y, float dir)
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
void GameModel::addBallReplacement(float x, float y, float vx, float vy)
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
void GameModel::notifyObservers()
{
    //std::cout << "at GameModel::notifyObservers()\n";
    setRobotHasBall();
    Team* t = Team::getTeam(TEAM_BLUE);
    if(t->isControlled()) t->controller.run();
    t = Team::getTeam(TEAM_YELLOW);
    if(t->isControlled()) t->controller.run();
}

/*! @brief Used by RefComm; Set the current game state (See RefComm reference)
 * @see RefComm
 * @see getGameState */
void GameModel::setGameState(char gameState)
{
    char lastGameState = this->gameState;
    if(lastGameState != gameState)
    {
        hasNewCommand = true;
        previousGameState = lastGameState;
    }
    this->gameState = gameState;
}

//! @brief Callback called from StrategyController when a new command is processed
void GameModel::onCommandProcessed()
{
    this->hasNewCommand = false;
}

//! @brief Sets the position of the ball
void GameModel::setBallPoint(Point bp)
{
    ballPoint = bp;
}

//! @brief Sets the velocity of the ball
void GameModel::setBallVelocity(Point vel)
{
    ballVelocity = vel;
}

//! @brief Sets the stopping point of the ball
void GameModel::setBallStopPoint(Point stop_point)
{
    ballStopPoint = stop_point;
}

static bool hasBall(Robot* robot)
{
    Point bp = gameModel->getBallPoint();
    if(!robot) 
        return false;
    return Comparisons::isDistanceToLess(robot, bp, 300) and
           Comparisons::isFacingPoint(robot, bp);
}

//! @brief Sets the robot that has currently been determined to have the ball.
void GameModel::setRobotHasBall()
{
    //Count of how many times `robotWithBall` has been seen without ball
    static int lastSeenWithoutBallCount = 0;

    //Assume no robot has the ball first
    auto robots = Robot::getAllRobots();
    for(Robot* robot : robots)
        robot->hasBall = false;

    if(!hasBall(this->robotWithBall) and ++lastSeenWithoutBallCount > 10)
    {
        lastSeenWithoutBallCount = 0;

        auto ballBot = std::find_if(robots.begin(), robots.end(), hasBall);
        if(ballBot == robots.end()) {            //Not found in myTeam
            this->robotWithBall = NULL;
            return;
        }
        this->robotWithBall = *ballBot;          //Robot with ball found, store in gm
    }

    if(robotWithBall)
        this->robotWithBall->hasBall = true;
}

//! @brief Used by RefComm; Set the current game state (See RefComm reference)
void GameModel::setTimeLeft(short time)
{
    remainingTime = time;
}

//! @brief Used by RefComm; Set the number of blue team goals
void GameModel::setBlueGoals(char goals)
{
    blueGoals = goals;
}

//! @brief Used by RefComm; Set the number of yellow team goals
void GameModel::setYellowGoals(char goals)
{
    yellowGoals = goals;
}














