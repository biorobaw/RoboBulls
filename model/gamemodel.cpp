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

static std::fstream file("/home/jameswaugh/data.txt", ios_base::out);


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

#define MEASURE(x) if(posedge(bs > x)) { std::cout << #x << std::endl; }

/* Called by VisionComm */
/* Don't overlook this function, it's more important than you think
 */
void GameModel::notifyObservers()
{
    static float bs = 0;
    static float max = 0;

     bs = gameModel->getBallSpeed();

    if(bs > 0.5) {
        max = std::max(max, bs);
        std::cout << max << " " << bs << std::endl;
    }

    if(posedge(bs < 1)) {
        file << max << " " << getBallPoint().toString() << std::endl;
        max = 0;
    }

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
    static float bs;
    bs = gameModel->getBallSpeed();

    Point prediction = gameModel->getBallPoint();

    //Calculate once, if the ball is starting to move
    if( posedge(bs > 1.5) )
    {
        Point bp = gameModel->getBallPoint();
        Point bv = gameModel->getBallVelocity() * POINTS_PER_METER;
        //Point ba = gameModel->getBallAcceleration() * METERS_PER_POINT;
        float ba = -0.1;
        float t = 3;

        float px = bp.x + bv.x*t + ba*t*t*0.5;
        float py = bp.y + bv.y*t + ba*t*t*0.5;

        prediction = Point(px, py);

        // Takin this out for video - too noisy

        // Also: this should be in gui main loop, not game model. martin
        GuiInterface::getGuiInterface()->drawPath(gameModel->getBallPoint(), prediction);

        //BUT MARTIN: The "drawPath" function was explicitly added by Ryan to draw arbitrary lines from our code.
    }

    return prediction;
}

void GameModel::setBallPoint(Point bp)
{
    //Sets ball point, and calculates velocity, acceleration, and the prediction.
    static VelocityCalculator ballVelCalculator(0);
    static VelocityCalculator ballAclCalculator(0);
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
