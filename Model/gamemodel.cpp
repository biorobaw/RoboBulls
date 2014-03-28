#include "gamemodel.h"

GameModel::GameModel(StrategyController * sc)
{
    this->sc = sc;
    gameState = '\0';
}


GameModel * GameModel::getModel(){
    if (model == 0)
        model = new GameModel();

    return model;
}

void GameModel::setOponentTeam(Robot** robot)
{

    for (int i=0; i < MAX_ROBOTS; i++)
    {
        oponentTeam[i] = robot[i];
    }
}

void GameModel::setMyTeam(Robot** robot)
{

    for (int i=0; i < MAX_ROBOTS; i++)
    {
        myTeam[i] = robot[i];
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

Robot ** GameModel::getOponentTeam()
{
    return oponentTeam;
}

Robot **GameModel::getMyTeam()
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


