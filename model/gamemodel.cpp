#include <sstream>
#include <iostream>
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

//Multiplier indicating which side of the field we/they are on
#if TEAM==TEAM_BLUE
int GameModel::mySide = -1;
int GameModel::opSide =  1;
#else
int GameModel::mySide =  1;
int GameModel::opSide = -1;
#endif

/*******************************************************************/
/************************ Public Methods ***************************/
/*******************************************************************/


GameModel::GameModel()
{
    gameState  = '\0';
}

void GameModel::setStrategyController(StrategyController *sc)
{
    this->sc = sc;
}

Robot* GameModel::getHasBall()
{
    return robotWithBall;
}

Robot* GameModel::findMyTeam(int id)
{
    return find(id, myTeam);
}

Robot* GameModel::findOpTeam(int id)
{
    return find(id, opTeam);
}

vector<Robot*>& GameModel::getOponentTeam()
{
    return opTeam;
}

vector<Robot *>& GameModel::getMyTeam()
{
    return myTeam;
}

Point GameModel::getBallPoint()
{
    return ballPoint;
}

Point GameModel::getBallVelocity()
{
    return ballVelocity;
}

Point GameModel::getBallAcceleration()
{
    return ballAcceleration;
}

float GameModel::getBallSpeed()
{
    return std::hypot(ballVelocity.x, ballVelocity.y);
}

bool GameModel::getBallIsStopped()
{
    return ballStopped;
}

Point GameModel::getBallPrediction()
{
    return ballPrediction;
}

char GameModel::getBlueGoals()
{
    return blueGoals;
}

char GameModel::getYellowGoals()
{
    return yellowGoals;
}

short GameModel::getRemainingTime()
{
    return remainingTime;
}

char GameModel::getGameState()
{
    return gameState;
}

GameModel * GameModel::getModel()
{
    return gameModel;
}

bool GameModel::isNewCommand()
{
    return this->hasNewCommand;
}

Point GameModel::getPenaltyPoint()
{
#if TEAM == TEAM_BLUE
        return Point(2045, 22);
#else
        return Point(-2045, 22);
#endif
}

Point GameModel::getOpponentGoal()
{
#if TEAM == TEAM_BLUE
        return Point(3000,0);
#else
        return Point(-3000, 0);
#endif
}

Point GameModel::getMyGoal()
{
#if TEAM == TEAM_BLUE
        return Point(-3000, 0);
#else
        return Point(3000,0);
#endif
}

char GameModel::getPreviousGameState()
{
    return previousGameState;
}

std::string GameModel::toString()
{
    stringstream myString;

    myString << "Ball Position: " << ballPoint.toString() <<endl;

    myString<<"\nMy Team Robots: \n";
    for (vector<Robot*>::iterator it = myTeam.begin(); it != myTeam.end(); it++)
    {
        myString << "\t" << (*it)->toString()<<endl;
    }

    myString<<"\nOponent Team Robots: \n";
    for (vector<Robot*>::iterator it = opTeam.begin(); it != opTeam.end(); it++)
    {
        myString << "\t" << (*it)->toString() <<endl;
    }

    return myString.str();
}


/*******************************************************************/
/************************ Private Methods **************************/
/*******************************************************************/


/* Called by VisionComm */
/* Don't overlook this function, it's more important than you think
 */
void GameModel::notifyObservers()
{
    setRobotHasBall();
    sc->run();
}

/* Called by RefComm */
void GameModel::setGameState(char gameState)
{
    char lastGameState = this->gameState;
    if(lastGameState != gameState)
    {
        hasNewCommand = true;
        setPreviousGameState(lastGameState);
    }
    this->gameState = gameState;
}

void GameModel::setPreviousGameState(char lastGameState)
{
    previousGameState = lastGameState;
}

void GameModel::onCommandProcessed()
{
    this->hasNewCommand = false;
}

static Point calcBallPrediction()
{
    //p = p0 + vt, with t = 2
    Point bp = gameModel->getBallPoint();
    Point predict = bp + gameModel->getBallVelocity() * POINTS_PER_METER * 2.0;
    return predict;
}

void GameModel::setBallPoint(Point bp)
{
    //Sets ball point, and calculates velocity, acceleration, and the prediction.
    static VelocityCalculator ballVelCalculator;
    static VelocityCalculator ballAclCalculator;
    ballPoint = bp;
    ballVelocity = ballVelCalculator.update(bp);
    ballAcceleration = ballAclCalculator.update(ballVelocity * POINTS_PER_METER);
    ballPrediction = calcBallPrediction();
}


static bool calculateHasBall(Robot* robot) 
{
    Point bp = gameModel->getBallPoint();
    if(!robot) 
        return false;
    return Comparisons::isDistanceToLess(robot, bp, 300) and
           Comparisons::isFacingPoint(robot, bp);
}

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

void GameModel::setTimeLeft(short time)
{
    remainingTime = time;
}

void GameModel::setBlueGoals(char goals)
{
    blueGoals = goals;
}

void GameModel::setYellowGoals(char goals)
{
    yellowGoals = goals;
}

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


static void calculateRobotVelocity(Robot* robot) 
{
    static VelocityCalculator robotVelCalcs[20];
    int   index  = (10 * robot->isOnMyTeam()) + robot->getID();
    Point newVel = robotVelCalcs[index].update(robot->getRobotPosition());
    robot->setVelocity(newVel);
}

void GameModel::onRobotUpdated(Robot* robot)
{
    calculateRobotVelocity(robot);
}
