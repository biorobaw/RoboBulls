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
bool GameModel::OUR_TEAM = TEAM_BLUE;

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
    return myTeam.getRobot(id);
}

/*! @brief Look for a robot with id `id` on getOponentTeam()
 * @param id The id of the robot to look for
 * \return A robot pointer if found, or NULL if not on the team */
Robot* GameModel::findOpTeam(int id)
{
    return opTeam.getRobot(id);
}

/*! @brief Return a vector of all robots on the opposing team
 * \return The opposing team (getBlueTeam() if Yellow, getYellowTeam() if Blue) */
Team& GameModel::getOppTeam()
{
    std::lock_guard<std::mutex> opp_team_guard(opp_team_mutex);
    return opTeam;
}

/*! @brief Return a vector of all robots on the current team
 * \return The current team (getBlueTeam() if Blue, getYellowTeam() if Yellow) */
Team& GameModel::getMyTeam()
{
    std::lock_guard<std::mutex> my_team_guard(my_team_mutex);
    return myTeam;
}

/*! @brief Return the team specified by the color
 * \return The Blue team */
Team& GameModel::getTeam(int color)
{

    return myTeam.color == color ? getMyTeam() : getOppTeam();
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

    myString<<"\nMy Team Robots: \n";
    auto robots = myTeam.getRobots();
    for (auto it = robots.begin(); it != robots.end(); it++)
    {
        myString << "\t" << (*it)->toString()<< std::endl;
    }

    myString<<"\nOponent Team Robots: \n";
    robots = opTeam.getRobots();
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
        Robot* robot = getTeam(team).getRobot(id);
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
    for(Robot* robot : myTeam.getRobots())
        robot->hasBall = false;
    for(Robot* robot : opTeam.getRobots())
        robot->hasBall = false;

    if(!hasBall(this->robotWithBall) and ++lastSeenWithoutBallCount > 10)
    {
        lastSeenWithoutBallCount = 0;
        auto robots = myTeam.getRobots();
        auto ballBot = std::find_if(robots.begin(), robots.end(), hasBall);
        if(ballBot == robots.end()) {            //Not found in myTeam

            robots = opTeam.getRobots();
            ballBot = std::find_if(robots.begin(), robots.end(), hasBall);
            if(ballBot == robots.end()) {        //Not found in opTeam
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

///*! @brief General-case find function
// * Looks for a robot with id `id` in a vector of robots. Similar to std::find_if.
// * @param id The Id to look for
// * @param team The team to look in (either getMyTeam or getOponentTeam)
// * @see findMyTeam
// * @see findOpTeam
// * @return A Robot pointer pointing into `team` if found, or NULL if not found
// */
//Robot* GameModel::find(int id, std::vector<Robot*>& team)
//{
//    /* Often, the vision system (and also almost always on the simulator)
//     * seems to report robots in order anyway. So first,
//     * I think it would be reasonable to check if the team at that `id`
//     * is actually that robot first.
//     */
//    try {
//        if(team.at(id)->getID() == id)
//            return team[id];
//    }
//    catch(...) {
//    }

//    for(Robot* rob : team)
//    {
//        if(rob->getID() == id)
//            return rob;
//    }

//    return NULL;
//}


/*! @brief Removes a robot from a team
 * Should not be used directly. Used to provide an interface for utilities/debug.h
 * "remove_robot" command. Use that instead.
 * @param id The Id to remove
 * @param team The team (TEAM_YELLOW) or (TEAM_BLUE) to remove from
 * @see utilities/debug.h */
void GameModel::removeRobot(int id, int team)
{
    getTeam(team).removeRobot(id);
}


void  GameModel::setTeams(YAML::Node team_node){

    myTeam = Team(team_node);

    GameModel::OUR_TEAM = myTeam.color;
    opTeam.color = 1 - myTeam.color;
    opTeam.side = 1 - myTeam.side;

}












