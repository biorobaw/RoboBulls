#include "strategy/strategy.h"
#include "movement/move.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "strategy/stopstrategy.h"
#include "strategy/teststrategy.h"
#include "strategy/kickoffstrategy.h"
#include "strategy/freekickstrategy.h"
#include "strategy/normalgamestrategy.h"
#include "strategy/penaltystrategy.h"
#include "strategy/indirectkickstrategy.h"
#include "strategy/haltstrategy.h"
#include "communication/robcomm.h"
#include "gui/guiinterface.h"
#include "strategycontroller.h"

StrategyController::StrategyController(GameModel* gm, bool refbox_enabled)
{
    activeStrategy = nullptr;
    model = gm;
    this->refbox_enabled = refbox_enabled;
    std::cout << "--STRATEGY " << std::endl ;
    std::cout << "        Refboxed: " << this->refbox_enabled << std::endl;
    std::cout << "--Strategy DONE" << std::endl;
}

void StrategyController::run()
{
    //StrategyController runs only if at least one robot is on the team.
    //This is for the initial reading at which robots may not be detected yet
//    std::cout<< "at StrategyController::run() \n";
    if(!model->getMyTeam().getRobots().empty())
    {
//        std::cout<< "at StrategyController::run() - team not empty\n";
        if(model->isNewCommand() || activeStrategy == nullptr) {
            //std::cout<< "at StrategyController::run()  resetting\n";
            gameModelReset();
        } else {
            //std::cout<< "at StrategyController::run() continued\n";
            gameModelContinued();
        }
        //std::cout<<"model->onCommandProcessed();"<<std::endl;
        model->onCommandProcessed();

        frameEnd();
    }
    //std::cout<<"OUTSIDE IF"<<std::endl; Bowu
}

void StrategyController::assignNewStrategy(char gameState)
{
    clearCurrentStrategy();

    if(refbox_enabled){
        std::cout << "Active strategy: "<<gameState<<std::endl;
        //gameState='K';//Added by Bo Wu
        switch(gameState) {
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
                activeStrategy = new IndirectKickStrategy();
                break;
            case 'T':
            case 't':
            case 'H':    //Halt
                activeStrategy = new HaltStrategy();
                break;
            case ' ':    //Normal game play
                std::cout << "Warning: GS \"" <<  gameState << "\" Not implemented" << std::endl;
                activeStrategy = new NormalGameStrategy();
                break;
            case 's':    //Force Start
                activeStrategy = new NormalGameStrategy();
                break;
            default:    //Anything Else
                std::cout << "Warning: GS \"" <<  gameState << "\" Not implemented" << std::endl;
                activeStrategy = new StopStrategy();
        };
    }
    else {
        (void)(gameState);
        activeStrategy = new TestStrategy();
        std::cout << "Active strategy: TestStrategy\n";
    }
}


void StrategyController::gameModelReset()
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
        // Recreate current strategy if requested
        if(clearStrategyFlag)
            clearCurrentStrategy();
        // Change the current strategy arbitrarily if requested
        else if(nextStrategyState != '\0' and
                nextStrategyState != currentGameState)
            assignNewStrategy(nextStrategyState);
    }
}

void StrategyController::clearCurrentStrategy()
{
    delete activeStrategy;
    activeStrategy = nullptr;
    for(Robot* robot : model->getMyTeam().getRobots())
        robot->clearBehavior();
}

void StrategyController::frameEnd()
{
    //std::cout << " In StrategyController::frameEnd()" <<std::endl;
    for (Robot *rob :  model->getMyTeam().getRobots())
    {
        if (!GuiInterface::getGuiInterface()->isOverriddenBot()[rob->getID()]) {
            if(rob->hasBehavior())
                rob->getBehavior()->perform(rob);
         }
    }

    RobComm::sendVels(model->getMyTeam().getRobots());
    //std::cout<<"frameEnd() get called\n"<<std::endl;
}
