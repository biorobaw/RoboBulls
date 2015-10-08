#include <sstream>
#include <iostream>
#include <fstream>
#include "include/config/team.h"
#include "include/config/simulated.h"
#include "strategy/strategycontroller.h"
#include "utilities/comparisons.h"
#include "utilities/velocitycalculator.h"
#include "utilities/edges.h"
#include "utilities/debug.h"
#include "gui/guiinterface.h"
#include "model/gamemodel.h"

// Global static pointer used to ensure a single instance of the class.
GameModel* gameModel = new GameModel();

/*******************************************************************/
/************************ Public Methods ***************************/
/*******************************************************************/

//! @brief Sets a StrategyController to run the game with
void GameModel::setStrategyController(StrategyController *sc)
{
    this->sc = sc;
}

/*! @brief Returns the robot that currently has the ball
 * A robot has the ball if it is close to it and is facing it */
Robot* GameModel::getHasBall()
{
    return robotWithBall;
}

/*! @brief Look for a robot with id `id` on getMyTeam()
 * @param id The id of the robot to look for
 * @see Robot class
 * \return A robot pointer if found, or NULL if not on the team */
Robot* GameModel::findMyTeam(int id)
{
    return find(id, myTeam);
}

/*! @brief Look for a robot with id `id` on getOponentTeam()
 * @param id The id of the robot to look for
 * \return A robot pointer if found, or NULL if not on the team */
Robot* GameModel::findOpTeam(int id)
{
    return find(id, opTeam);
}

/*! @brief Return a vector of all robots on the opposing team
 * \return The opposing team (getBlueTeam() if Yellow, getYellowTeam() if Blue) */
std::vector<Robot*>& GameModel::getOponentTeam()
{
    return opTeam;
}

/*! @brief Return a vector of all robots on the current team
 * \return The current team (getBlueTeam() if Blue, getYellowTeam() if Yellow) */
std::vector<Robot*>& GameModel::getMyTeam()
{
    return myTeam;
}

/*! @brief Return a vector of all robots on the Blue team
 * \return The Blue team */
std::vector<Robot*>& GameModel::getBlueTeam()
{
#if TEAM == TEAM_BLUE
    return getMyTeam();
#else
    return getOponentTeam();
#endif
}

/*! @brief Return a vector of all robots on the Yellow team
 * \return The Yellow team */
std::vector<Robot*>& GameModel::getYellowTeam()
{
#if TEAM == TEAM_BLUE
    return getOponentTeam();
#else
    return getMyTeam();
#endif
}

/*! @brief Return a 2D point of the current ball location
 * \return A Point with the ball's position */
Point GameModel::getBallPoint()
{
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

/*! @brief Return a 2D point of the X and Y component of the ball's acceleration
 * \see getBallVelocity
 * \see VelocityCalculator
 * \return The ball's acceleration as an X/Y point in m/s*/
Point GameModel::getBallAcceleration()
{
    return ballAcceleration;
}

/*! @brief Returns the ball's <i>speed</i> (magnitude of velocity) in m/s */
float GameModel::getBallSpeed()
{
    return std::hypot(ballVelocity.x, ballVelocity.y);
}

/*! @brief Returns true of the ball is not moving, or false if it is moving */
bool GameModel::getBallIsStopped()
{
    return ballStopped;
}

/*! @brief Returns a predition of the ball's position in 2 seconds
 * The Ball Prediciton is a rudimentary kinematics calculation that takes into
 * account the position and velocity.
 * \see getBallPoint
 * \see getBallVelocity
 * \return The ball's expected location in about two seconds. */
Point GameModel::getBallPrediction()
{
    return ballPrediction;
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
Point GameModel::getOpponentGoal()
{
    return Point(FIELD_LENGTH, 0);
}

//! @brief Returns the goal point that we are defending (edge of the field)
Point GameModel::getMyGoal()
{
    return Point(-FIELD_LENGTH, 0);
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

    myString<<"\nMy Team Robots: \n";
    for (auto it = myTeam.begin(); it != myTeam.end(); it++)
    {
        myString << "\t" << (*it)->toString()<< std::endl;
    }

    myString<<"\nOponent Team Robots: \n";
    for(auto it = opTeam.begin(); it != opTeam.end(); it++)
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
        Robot* robot = gameModel->find(id, (team == TEAM_BLUE) ? getBlueTeam() : getYellowTeam());
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
 * StrategyController and make the project work. This also update the GUI system. */
void GameModel::notifyObservers()
{
    setRobotHasBall();
    sc->run();
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

//! @brief To do the actual calculation of the ball's predicted (stopping) point
static Point calcBallPrediction(float t)
{
    //p = p0 + vt + (1/2)at^2, with t = some t
    Point predict_t =
              gameModel->getBallPoint()
            + gameModel->getBallVelocity() * POINTS_PER_METER * t
            + gameModel->getBallAcceleration()*POINTS_PER_METER * t * t * 0.5;
    return predict_t;
}

//! @brief Sets the position of the ball. Updates velocity and acceleration
void GameModel::setBallPoint(Point bp)
{
    //Sets ball point, and calculates velocity, acceleration, and the prediction.
    static VelocityCalculator ballVelCalculator(10);
    static VelocityCalculator ballAclCalculator(3);
    ballPoint = bp;
    ballVelocity = ballVelCalculator.update(bp);
    ballAcceleration = ballAclCalculator.update(ballVelocity);
    ballPrediction = calcBallPrediction(3.2);
}


static bool calculateHasBall(Robot* robot) 
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
    for(Robot* robot : myTeam)
        robot->hasBall = false;
    for(Robot* robot : opTeam)
        robot->hasBall = false;

    if(!calculateHasBall(this->robotWithBall) and ++lastSeenWithoutBallCount > 10) 
    {
        lastSeenWithoutBallCount = 0;
        auto ballBot = std::find_if(myTeam.begin(), myTeam.end(), calculateHasBall);
        if(ballBot == myTeam.end()) {            //Not found in myTeam
            ballBot = std::find_if(opTeam.begin(), opTeam.end(), calculateHasBall);
            if(ballBot == opTeam.end()) {        //Not found in opTeam
                this->robotWithBall = NULL;
                return;
            }
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

/*! @brief General-case find function
 * Looks for a robot with id `id` in a vector of robots. Similar to std::find_if.
 * @param id The Id to look for
 * @param team The team to look in (either getMyTeam or getOponentTeam)
 * @see findMyTeam
 * @see findOpTeam
 * @return A Robot pointer pointing into `team` if found, or NULL if not found
 */
Robot* GameModel::find(int id, std::vector<Robot*>& team)
{
    /* Often, the vision system (and also almost always on the simulator)
     * seems to report robots in order anyway. So first,
     * I think it would be reasonable to check if the team at that `id`
     * is actually that robot first.
     */
    try {
        if(team.at(id)->getID() == id)
            return team[id];
    }
    catch(...) {
    }

    for(Robot* rob : team)
    {
        if(rob->getID() == id)
            return rob;
    }

    return NULL;
}

/*! @brief Stores a VelocityCalculator for each robot, and updates each robot's velocitiy.
 * This is called when the VisionComm says a robot has been updated */
static Point calculateRobotVelocity(Robot* robot)
{
    static VelocityCalculator robotVelCalcs[20];
    int   index  = (10 * robot->isOnMyTeam()) + robot->getID();
    Point newVel = robotVelCalcs[index].update(robot->getRobotPosition());
    return newVel;
}

//! @brief Used by VisiomComm; Update the information in the specified robot on the specified team
void GameModel::onRobotUpdated(Robot* robot)
{
    robot->setVelocity(calculateRobotVelocity(robot));
}

/*! @brief Removes a robot from a team
 * Should not be used directly. Used to provide an interface for utilities/debug.h
 * "remove_robot" command. Use that instead.
 * @param id The Id to remove
 * @param team The team (TEAM_YELLOW) or (TEAM_BLUE) to remove from
 * @see utilities/debug.h */
void GameModel::removeRobot(int id, int team)
{
    auto* vector = &getMyTeam();
    if(team != TEAM)
        vector = &getOponentTeam();
    auto it = std::find_if(vector->begin(), vector->end(), [=](Robot* r){return r->getID()==id;});
    if(it != vector->end()) {
        delete *it; //Free the Robot* pointer first
        vector->erase(it);
    }
}
