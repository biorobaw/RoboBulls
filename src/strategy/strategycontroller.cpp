#include "strategycontroller.h"
#include "strategy/strategy.h"
#include "model/game_state.h"
#include "robot/robot.h"
#include "model/team.h"
#include "gui/gui_interface.h"

#include "controllers/scontroller_normal_game.h"
#include "controllers/strategy_tester/scontroller_strategy_tester.h"
#include "controllers/joystick/scontroller_joystick.h"
#include "strategies/haltstrategy.h"
#include "yaml-cpp/yaml.h"

#include <string>
#include <iostream>
using std::cerr, std::endl, std::string;

StrategyController::StrategyController( RobotTeam* _team, YAML::Node* n)
  : team(_team),
    activeStrategy(new HaltStrategy(_team))
{
    (void)n;
}
StrategyController::~StrategyController(){}

void StrategyController::run()
{
    // update strategy if necessary
    int new_state = received_new_command ?
                            getControllerState(GameState::getRefereeCommand()) :
                            getNextControllerState(controller_state, activeStrategy->getStatus());
    received_new_command = false;

    if(new_state!=controller_state){
        controller_state = new_state;
        delete activeStrategy;
        for(Robot* r : team->getRobots()) r->clearBehavior();
        activeStrategy = loadStateStrategy(controller_state);
        activeStrategy->assignBehaviors();
    }


    // update strategy
    activeStrategy->update();

    // perform robot behaviors
    for (Robot *rob :  team->getRobots())
        if (!GuiInterface::getGuiInterface()->isOverriddenBot(team->getColor(),rob->getID())) {
//            std::cout << "R" << rob->getID() << " not overriden, has beh? " << rob->hasBehavior() << std::endl;
            rob->performBehavior();

         }


    // send velocities to the robots
    team->sendVels();

}


void StrategyController::signalNewCommand(){
    received_new_command = true;
}



StrategyController* StrategyController::loadController(RobotTeam* team, YAML::Node* c_node){

    std::cout << "        STRATEGY_CONTROLLER  : " <<std::endl;
    std::cout << "            ID : " <<  (*c_node)["ID"] << std::endl;
    auto id = (*c_node)["ID"].as<string>();

    if(id == "NORMAL_GAME"){
        return new SControllerNormalGame(team,c_node);
    } else if( id == "STRATEGY_TESTER"){
        return new SControllerStrategyTester(team,c_node);
    } else if ( id == "JOYSTICK" ) {
        return new SControllerJoystick(team,c_node);
    }else {
        cerr << "ERROR : controller '" << id << "' not recognized" <<endl
             << "\tHalting execution...";
        exit(-1);
    }


}

std::string StrategyController::getStrategyName(){
    return activeStrategy == nullptr ? "null" : activeStrategy->getName();
}
