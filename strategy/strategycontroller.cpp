#include "strategy/strategy.h"
#include "movement/move.h"
#include "model/game_state.h"
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
#include "model/game_state.h"

StrategyController::StrategyController( Team* _team) : team(_team) {
}

void StrategyController::run()
{
    //StrategyController runs only if at least one robot is on the team.
    //This is for the initial reading at which robots may not be detected yet
//    std::cout<< "at StrategyController::run() \n";
    if(!team->getRobots().empty())
    {
//        std::cout<< "at StrategyController::run() - team not empty\n";
        if(gameState->isNewCommand() || activeStrategy == nullptr) {
            //std::cout<< "at StrategyController::run()  resetting\n";
            gameModelReset();
        } else {
            //std::cout<< "at StrategyController::run() continued\n";
            gameModelContinued();
        }
        //std::cout<<"model->onCommandProcessed();"<<std::endl;
        gameState->onCommandProcessed();

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
                activeStrategy = new StopStrategy(team);
                break;
            case 'p':   //Yellow Penalty Kick
            case 'P':   //Blue Penalty Kick
                activeStrategy = new PenaltyStrategy(team);
                break;
            case 'k':   //Yellow Kickoff
            case 'K':   //Blue Kickoff
                activeStrategy = new KickOffStrategy(team);
                break;
            case 'f':   //Yellow Free Kick
            case 'F':   //Blue Free Kick
                activeStrategy = new FreeKickStrategy(team);
                break;
            case 'i':   //Yellow Indirect Kick
            case 'I':   //Blue Indirect kick
                activeStrategy = new IndirectKickStrategy(team);
                break;
            case 'T':
            case 't':
            case 'H':    //Halt
                activeStrategy = new HaltStrategy(team);
                break;
            case ' ':    //Normal game play
                std::cout << "Warning: GS \"" <<  gameState << "\" Not implemented" << std::endl;
                activeStrategy = new NormalGameStrategy(team);
                break;
            case 's':    //Force Start
                activeStrategy = new NormalGameStrategy(team);
                break;
            default:    //Anything Else
                std::cout << "Warning: GS \"" <<  gameState << "\" Not implemented" << std::endl;
                activeStrategy = new StopStrategy(team);
        };
    }
    else {
        (void)(gameState);
        activeStrategy = new TestStrategy(team);
        std::cout << "Active strategy: TestStrategy\n";
    }
}


void StrategyController::gameModelReset()
{
    clearCurrentStrategy();
    char newState = gameState->getState();
    this->assignNewStrategy(newState); //Updates activeStrategy
    activeStrategy->assignBeh();
}


void StrategyController::gameModelContinued()
{
    if(activeStrategy != nullptr)
    {
        bool clearStrategyFlag = activeStrategy->update() == true;
        char nextStrategyState = activeStrategy->getNextStrategy();
        char currentGameState  = gameState->getState();
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
    for(Robot* robot : team->getRobots())
        robot->clearBehavior();
}

void StrategyController::frameEnd()
{
    //std::cout << " In StrategyController::frameEnd()" <<std::endl;
    for (Robot *rob :  team->getRobots())
    {
        if (!GuiInterface::getGuiInterface()->isOverriddenBot()[rob->getID()]) {
            if(rob->hasBehavior())
                rob->getBehavior()->perform(rob);
         }
    }

    RobComm::sendVels(team->getRobots());
    //std::cout<<"frameEnd() get called\n"<<std::endl;
}


Team* StrategyController::getTeam(){
    return team;
}

void StrategyController::setRefboxEnabled(bool _enabled){
    refbox_enabled = _enabled;
}
