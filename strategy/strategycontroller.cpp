#include "strategycontroller.h"
#include "strategy/strategy.h"

#include "model/gamemodel.h"
#include "model/robot.h"
#include "behavior/behavior.h"
#include "strategy/stopstrategy.h"
#include "strategy/teststrategy.h"
#include "strategy/penaltystrategy.h"
#include "strategy/kickoffstrategy.h"
#include "strategy/freekickstrategy.h"
#include "strategy/haltstrategy.h"
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

    /* Adjustment: The new vision system (11/7/14) requirements
     * (Seeing a robot X amounts of times before it is added) behaves
     * poorly with strategies with assignBeh only. assignBeh is only called once,
     * and that is _before_ 50 frames have passed, and so no robots will be
     * on the team. Then, assignBeh will never be called again.
     * Here we're only going to run if there are robots on the team.
     * Also, this is a non-bad way of "not doing anything" until the game
     * is in a valid state.
     * And, this means nothing will work if there are no robots.
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
    activeStrategy = new NormalGameStrategy();
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
