#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "communication/robcomm.h"
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
#include "strategy/videostrategies.h"
#include "movement/pathfinding/fppa_pathfinding.h"
#include "gui/guiinterface.h"
#include "strategycontroller.h"

//! @brief Chooses between listening to RefComm,and always choosing TestStrategy.
#define USE_TEST_STRATEGY 0


StrategyController::StrategyController(GameModel* gm)
{
    activeStrategy = nullptr;
    model = gm;
}

void StrategyController::run()
{
    //StrategyController runs only if at least one robot is on the team.
    //This is for the initial reading at which robots may not be detected yet
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

void StrategyController::assignNewStrategy(char gameState)
{
    clearCurrentStrategy();

    /* Testing macro: Change this to 0 to ignore game sate
     * commands; use to test a single strategy
     */
#if !USE_TEST_STRATEGY
    switch(gameState)
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
    case 'T':
    case 't':
    case 'H':    //Halt
        activeStrategy = new HaltStrategy();
        break;
    case ' ':    //Normal game play
        activeStrategy = new NormalGameStrategy();
        break;
    case 's':    //Force Start
        activeStrategy = new NormalGameStrategy();
        break;
    default:    //Anything Else
        activeStrategy = new TestStrategy();
    };
#else
    (void)(gameState);
    activeStrategy = new TestStrategy();
#endif
}


void StrategyController::gameModelUpdated()
{
    clearCurrentStrategy();
    char newState = model->getGameState();

    this->assignNewStrategy(newState); //Updates activeStrategy
    activeStrategy->assignBeh();
}


void StrategyController::gameModelContinued()
{
    if(activeStrategy != nullptr)
    {
        bool clearStrategyFlag = activeStrategy->update() == true;
        char nextStrategyState = activeStrategy->getNextStrategy();
        char currentGameState  = model->getGameState();
        //Recreate current strategy if requested
        if(clearStrategyFlag)
            clearCurrentStrategy();
        //Change the current strategy arbitrarily if requested
        else if(nextStrategyState != '\0' and
                nextStrategyState != currentGameState)
            assignNewStrategy(nextStrategyState);
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
    Movement::FPPA::update();
}

void StrategyController::frameEnd()
{
    for (unsigned int i=0; i < model->getMyTeam().size(); i++)
    {
        Robot *rob = model->getMyTeam().at(i);
        if (!GuiInterface::getGuiInterface()->isOverriddenBot()[rob->getID()]) {
            if(rob->hasBehavior())
                rob->getCurrentBeh()->perform(rob);
         }
    }

    RobComm * robcom = RobComm::getRobComm();
    robcom->sendVelsLarge(model->getMyTeam());
}
