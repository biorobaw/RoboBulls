#include "strategycontroller.h"
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
#include "strategy/attackstrategy.h"
#include "strategy/normalgamestrategy.h"
#include "strategy/indiectkickstrategy.h"
#include "strategy/normalgamestrategy.h"
#include "movement/pathfinding/fppa_pathfinding.h"


#include <QApplication>
#include "gui/guiinterface.h"

using namespace std;

StrategyController::StrategyController()
{
    activeStrategy = nullptr;
}

void StrategyController::run()
{
    static int count = 0;
    if(count < 25) { ++count; return; }

    frameBegin();
    
    if(model->isNewCommand() || activeStrategy==nullptr) {
        gameModelUpdated();
    } else {
        gameModelContinued();
    }

    model->onCommandProcessed();
    
    frameEnd();
}

void StrategyController::gameModelUpdated()
{
    clearCurrentStrategy();

    cout << model->getGameState() << endl;


    /* Testing macro: Change this to 0 to ignore refcom commands
     * to test a single strategy
     */
#if 1
    switch(model->getGameState())
    {
    case 'S':    //stop game
    case 'G':    //Blue Goal
    case 'g':    //Yellow Goal
        activeStrategy = new StopStrategy();
        break;
    case 'p':   //Yellow Penalty Kick
    case 'P':   //Blue Penalty Kick
        activeStrategy = new PenaltyStrategy();
        break;
    case 'k':   //Yellow Kickoff
    case 'K':   //Blue Kickoff
        activeStrategy = new KickOffStrategy();
        break;
    case 'f':   //Yellow Free Kick
    case 'F':   //Blue Free Kick
        activeStrategy = new FreeKickStrategy();
        break;
    case 'i':   //Yellow Indirect Kick
    case 'I':   //Blue Indirect kick
        activeStrategy = new IndiectKickStrategy();
        break;
    case 'H':    //Halt
        activeStrategy = new HaltStrategy();
        break;
    case ' ':    //Normal game play
        activeStrategy = new NormalGameStrategy();
        break;
    case 's':    //Force Start
        activeStrategy = new FreeKickStrategy();
        break;
    default:    //Anything Else
        activeStrategy = new TestStrategy();
    };
#else
    activeStrategy = new TestStrategy();
#endif

    activeStrategy->assignBeh();

}

void StrategyController::gameModelContinued()
{
    if(activeStrategy != nullptr) {
        bool clearStratFlag = activeStrategy->update();
        if(clearStratFlag)
            clearCurrentStrategy();
    }
}

void StrategyController::clearCurrentStrategy()
{
    delete activeStrategy;
    activeStrategy = nullptr;
    
    for(Robot* robot : model->getMyTeam())
        robot->clearCurrentBeh();
}


void StrategyController::frameBegin()
{
    FPPA::pathfindingBegin();
}


void StrategyController::frameEnd()
{
    for (unsigned int i=0; i < model->getMyTeam().size(); i++)
    {
        Robot *rob = model->getMyTeam().at(i);
        if (!GuiInterface::getGuiInterface()->isOverriddenBot()[i]) {
            if(rob->hasBeh)
                rob->getCurrentBeh()->perform(rob);
         }
    }

    RobComm * robcom = RobComm::getRobComm();
    robcom->sendVelsLarge(model->getMyTeam());
    FPPA::pathfindingEnd();
}


void StrategyController::setGameModel(GameModel *myGameModel)
{
    model = myGameModel;
}
