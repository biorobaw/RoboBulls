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
    /* Adjustment: The new vision system requirements
     * (Seeing a robot X amounts of times before it is added) behaves
     * poorly with strategies with assignBeh only. assignBehis only called once,
     * and that is _before_ 50 frames have passed, andso no robots will be
     * on the team. Then, assignBeh will never be called again.
     * Here we're only going to run if there are robots on the team.
     */
    if(!model->getMyTeam().empty())
    {
        frameBegin();

        if(model->isNewCommand() || activeStrategy==nullptr) {
            gameModelUpdated();
        } else {
            gameModelContinued();
        }

        model->onCommandProcessed();

        frameEnd();
    }
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
    case 'P':    //Penalty Kick
        activeStrategy = new PenaltyStrategy();
        break;
    case 'K':    //Kickoff
        activeStrategy = new KickOffStrategy();
        break;
    case 'F':    //Free Kick
        activeStrategy = new FreeKickStrategy();
        break;
    case 'I':   //Indirect kick
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
//        if (!GuiInterface::getGuiInterface()->isOverriddenBot()[i]) {
            if(rob->hasBeh)
                rob->getCurrentBeh()->perform(rob);
//         }
    }

    RobComm * robcom = RobComm::getRobComm();
    robcom->sendVelsLarge(model->getMyTeam());
    FPPA::pathfindingEnd();
}


void StrategyController::setGameModel(GameModel *myGameModel)
{
    model = myGameModel;
}
