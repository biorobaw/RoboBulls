#include "strategycontroller.h"
#include <iostream>

using namespace std;



StrategyController::StrategyController()
{
}

void StrategyController::gameModelUpdated()
{
    static int count = 0;
    if(count < 25) {++count; return;}
//    cout<< model->getGameState() << endl;

    if (model->getGameState() == 'S')
    {
        activeStrategy = new StopStrategy();
    }
    else if (model->getGameState() == 'p')
    {
        activeStrategy = new PenaltyStrategy();
    }
    else
    {
        activeStrategy = new StopStrategy();
    }

    activeStrategy->assignBeh();


    for (unsigned int i=0; i < model->getMyTeam().size(); i++)
    {
        Robot *rob = model->getMyTeam().at(i);

        if(rob->hasBeh)
         rob->getCurrentBeh()->perform(rob);
    }
}

void StrategyController::setGameModel(GameModel *myGameModel)
{
    model = myGameModel;
}
