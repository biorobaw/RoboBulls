#include "strategycontroller.h"
#include <iostream>

using namespace std;



StrategyController::StrategyController()
{
}

void StrategyController::gameModelUpdated()
{
    cout<< model->getGameState() << endl;

    if (model->getGameState() == 'S')
    {
        activeStrategy = new StopStrategy();
    } else {
        activeStrategy = new StopStrategy();
    }

    activeStrategy->assignBeh();

//    for (int i=0; i < MAX_ROBOTS; i++)
//    {

//    }

//    for each robot in myTeam
//      robot->currentBehaviour->perform()
}

void StrategyController::setGameModel(GameModel *myGameModel)
{
    model = myGameModel;
}
