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
        beh = new StopBehavior(); // TODO: This should not be here - it's stopstrategy
    } else {
        activeStrategy = new StopStrategy();
        beh = new StopBehavior(); // TODO: This should not be here - it's stopstrategy
    }

    activeStrategy->assignBeh();
    // USING ANY ROBOT TO MAKE IT WORK!!!!!!!!!!!! CHANGE!!!!!!!!!!!!!!!!

    cout << "Size   " << model->getMyTeam().size() << endl;


    for (int i=0; i < model->getMyTeam().size(); i++)
    {
        cout<<"sending perform!"<<endl;
        beh->perform(model->getMyTeam().at(i)); // TODO: robot->getBeh()->perform(robot)
                                                //       robot->performBeh();
    }
    cout<<"Game model updates!" << endl;
//    for each robot in myTeam
//      robot->currentBehaviour->perform()
}

void StrategyController::setGameModel(GameModel *myGameModel)
{
    model = myGameModel;
}
