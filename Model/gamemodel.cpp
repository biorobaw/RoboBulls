#include "gamemodel.h"
#include "Robot/robot.h"
#include <stddef.h>  // defines NULL

// Global static pointer used to ensure a single instance of the class.
GameModel* GameModel::model = NULL;

GameModel::GameModel()
{
   //this->sc = sc;
    gameState = '\0';
}


void GameModel::setOponentTeam(vector<Robot*> robot )
{

    for (int i=0; i < MAX_ROBOTS; i++)
    {
        oponentTeam.push_back(robot.at(i));
    }
}

void GameModel::setMyTeam(vector <Robot*> team)
{
    myTeam = team;
    sc->gameModelUpdated();
//    for (int i=0; i < MAX_ROBOTS; i++)
//    {
//        myTeam.push_back(robot[i]);
//    }
}

void GameModel::setXBall(float x)
{
    x_ballPosition = x;
}

void GameModel::setYball(float y)
{
    y_ballPosition = y;
}

void GameModel::setBallPoint(Point bp)
{
    ballPoint = bp;
}

void GameModel::setGameState(char gameState)
{
    char previousState = this->gameState;
    this->gameState = gameState;

    if (previousState != gameState)
    {
        sc->gameModelUpdated();
    }

}

void GameModel::setStrategyController(StrategyController *sc)
{
    this->sc = sc;
}

vector<Robot*> GameModel::getOponentTeam()
{
    return oponentTeam;
}

vector<Robot*> GameModel::getMyTeam()
{
    return myTeam;
}

float GameModel::getXBall()
{
    return x_ballPosition;
}

float GameModel::getYBall()
{
    return y_ballPosition;
}

Point GameModel::getBallPoint()
{
    return ballPoint;
}


int GameModel::getBlueGoals()
{
    return blueGoals;
}

int GameModel::getYellowGoals()
{
    return yellowGoals;
}

float GameModel::getRemainingTime()
{
    return remainingTime;
}

char GameModel::getGameState()
{
    return gameState;
}

GameModel * GameModel::getModel()
{
    if (model == NULL)
        model = new GameModel();

    return model;
}

//void GameModel::play()
//{
//    cout<<"Playing!"<<endl;
//    for (int i=0; i<MAX_ROBOTS; i++)
//    {
//        myTeam.at(i)->getCurrentBeh().perform(myTeam.at(i), gameBall);
//    }
//}



