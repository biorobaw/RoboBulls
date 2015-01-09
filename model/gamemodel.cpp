#include <stddef.h>  // defines NULL
#include <sstream>
#include "include/config/team.h"
#include "gamemodel.h"
#include "strategy/strategycontroller.h"
#include "utilities/comparisons.h"

// Global static pointer used to ensure a single instance of the class.
GameModel* gameModel = new GameModel();


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

unsigned char GameModel::getBlueGoals()
{
    return blueGoals;
}

unsigned char GameModel::getYellowGoals()
{
    return yellowGoals;
}

unsigned char GameModel::getRemainingTime()
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
/**************************************************/
/* Don't overlook this function, it's more important
 * than you think
 */
void GameModel::notifyObservers()
{
    setRobotHasBall();
    sc->run();
}
/**************************************************/

/* Called by RefComm */
/* I don't think both RefComm and VisionComm should both
 * make the game run. This opens up the possibility for running the
 * loop twice, and possible interference. Before, this function also
 * caused the StrategyController to update if the state was different
 */
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


void GameModel::setBallPoint(Point bp)
{
    ballPoint = bp;
}


void GameModel::setRobotHasBall()
{
    static int lastSeenWithoutBallCount = 0;

    auto calculateHasBall = [&](Robot* rob) {
        if(rob == NULL)
            return false;
        if(Comparisons::isDistanceToGreater(ballPoint, rob, 300))
            return false;
        if(Comparisons::isNotFacingPoint(rob, ballPoint))
            return false;
        return true;
        };

    //Assume no robot has the ball first
    for(Robot* robot : myTeam)
        robot->hasBall = false;
    for(Robot* robot : opTeam)
        robot->hasBall = false;


    if(!calculateHasBall(this->robotWithBall)) {
        if(++lastSeenWithoutBallCount > 10)
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
            this->robotWithBall = *ballBot;             //Robot with ball found, store in gm
        }
    }

    if(robotWithBall)
        this->robotWithBall->hasBall = true;
}

void GameModel::setTimeLeft(unsigned short time)
{
    remainingTime = time;
}

void GameModel::setBlueGoals(unsigned char goals)
{
    blueGoals = goals;
}

void GameModel::setYellowGoals(unsigned char goals)
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


void GameModel::setRobotUpdated(Robot* robot, int whichTeam)
{
#if MODEL_USE_AVERAGES
    #warning Model Averages is deprecated
#else
    UNUSED_PARAM(robot);
    UNUSED_PARAM(whichTeam);
#endif
}
