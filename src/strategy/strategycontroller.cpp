#include "src/strategy/strategy.h"
#include "src/movement/move.h"
#include "src/model/game_state.h"
#include "src/model/robot.h"
#include "src/strategy/stopstrategy.h"
#include "src/strategy/teststrategy.h"
#include "src/strategy/kickoffstrategy.h"
#include "src/strategy/freekickstrategy.h"
#include "src/strategy/normalgamestrategy.h"
#include "src/strategy/penaltystrategy.h"
#include "src/strategy/indirectkickstrategy.h"
#include "src/strategy/haltstrategy.h"
#include "src/communication/robcomm.h"
#include "src/gui/guiinterface.h"
#include "strategycontroller.h"
#include "src/model/game_state.h"

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
        if(received_new_command || activeStrategy == nullptr) {
            //std::cout<< "at StrategyController::run()  resetting\n";

            assignNewStrategy(GameState::getState()); //Updates activeStrategy
            activeStrategy->assignBeh();

        } else {
            //std::cout<< "at StrategyController::run() continued\n";
            runActiveStrategy();
        }
        //std::cout<<"model->onCommandProcessed();"<<std::endl;
        received_new_command = false;

        sendRobotCommands();
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


void StrategyController::runActiveStrategy()
{
    if(activeStrategy != nullptr)
    {
        bool clearStrategyFlag = activeStrategy->update();
        char nextState = activeStrategy->getNextStrategy();
        // Recreate current strategy if requested or
        // Change the current strategy arbitrarily if requested
        if(clearStrategyFlag) clearCurrentStrategy();
        else if(nextState != '\0' && nextState != GameState::getState())
            assignNewStrategy(nextState);
    }
}

void StrategyController::clearCurrentStrategy()
{
    delete activeStrategy;
    activeStrategy = nullptr;
    for(Robot* robot : team->getRobots())
        robot->clearBehavior();
}

void StrategyController::sendRobotCommands()
{
    //std::cout << " In StrategyController::frameEnd()" <<std::endl;
    for (Robot *rob :  team->getRobots())
    {
        if (!GuiInterface::getGuiInterface()->isOverriddenBot(team->getColor(),rob->getID())) {
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

void StrategyController::signalNewCommand(){
    received_new_command = true;
}
