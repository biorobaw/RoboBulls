#include "strategycontroller.h"
#include <iostream>
#include "strategy/strategy.h"
#include "model/gamemodel.h"

#include "model/robot.h"
#include "behavior/behavior.h"

#include "strategy/stopstrategy.h"
#include "strategy/twovone.h"
#include "strategy/teststrategy.h"
#include "strategy/penaltystrategy.h"
#include "strategy/kickoffstrategy.h"
#include "strategy/freekickstrategy.h"
#include "strategy/haltstrategy.h"

#include "movement/pathfinding/fppa_pathfinding.h"

using namespace std;



StrategyController::StrategyController()
{
}

void StrategyController::gameModelUpdated()
{
    static int count = 0;
    if(count < 25) {++count; return;}
	
	FPPA::pathfindingBegin();

    //cout << model->getGameState() << endl;

    if (model->getGameState() == 'S')   //stop game
    {
        activeStrategy = new StopStrategy();
    }
    else if (model->getGameState() == 'P')  //penalty
    {
        activeStrategy = new PenaltyStrategy();
    }
    else if (model->getGameState() == 'K')  //kick off
    {
        activeStrategy = new KickOffStrategy();
    }
    else if (model->getGameState() == 'F')  //free kick
    {
        activeStrategy = new FreeKickStrategy();
    }
    else if (model->getGameState() == 'H')  //halt
    {
        activeStrategy = new HaltStrategy();
    }
    else if (model->getGameState() == ' ')  //normal start
    {
        activeStrategy = new TestStrategy();
    }
    else if (model->getGameState() == 's')  //force start
    {
        activeStrategy = new FreeKickStrategy();
    }
    else if (model->getGameState() == 'I')    //indirect kick
    {
        activeStrategy = new FreeKickStrategy();
    }
//    else if (model->getGameState() == 'G' || model->getGameState() == 'g')    //add blue and yellow goal
//    {
//        activeStrategy = new AddGoalStrategy();
//    }
    else
    {
//        activeStrategy = new TestStrategy();
        activeStrategy = new TwoVOne();
    }

    activeStrategy->assignBeh();


    for (unsigned int i=0; i < model->getMyTeam().size(); i++)
    {
        Robot *rob = model->getMyTeam().at(i);

        if(rob->hasBeh)
            rob->getCurrentBeh()->perform(rob);
    }

    RobComm * robcom = RobComm::getRobComm();
//    if(!sent) {
//        sent = true;
//        std::cout << "Send one" << std::endl;
        robcom->sendVelsLarge(model->getMyTeam());
//    }
	FPPA::pathfindingEnd();
	
	delete activeStrategy;
}

void StrategyController::setGameModel(GameModel *myGameModel)
{
    model = myGameModel;
}
