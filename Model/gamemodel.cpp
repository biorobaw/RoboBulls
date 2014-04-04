#include "gamemodel.h"
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

void GameModel::setMyTeam(vector <Robot*> robot)
{

    for (int i=0; i < MAX_ROBOTS; i++)
    {
        myTeam.push_back(robot[i]);
    }
}

void GameModel::setXBall(int x)
{
    x_ballPosition = x;
}

void GameModel::setYball(int y)
{
    y_ballPosition = y;
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

vector<Robot*> GameModel::getOponentTeam()
{
    return oponentTeam;
}

vector<Robot*> GameModel::getMyTeam()
{
    return myTeam;
}

int GameModel::getXBall()
{
    return x_ballPosition;
}

int GameModel::getYBall()
{
    return y_ballPosition;
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

GameModel * GameModel::getModel(){
    if (model == NULL)
        model = new GameModel();

    return model;
}

void GameModel::setStrategyController(StrategyController *sc){
    this->sc = sc;
}

