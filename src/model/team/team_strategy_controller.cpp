#include "team_strategy_controller.h"
#include "team_strategy.h"

#include "model/team/team.h"
#include "model/game_state.h"
#include "model/robot/robot.h"
#include "gui/interface/gui_interface.h"

#include "controllers/normal_game/scontroller_normal_game.h"
#include "controllers/strategy_tester/scontroller_strategy_tester.h"
#include "controllers/joystick/scontroller_joystick.h"
#include "controllers/none/scontroller_none.h"
#include "utilities/my_yaml.h"
#include <iostream>
#include <QDebug>


TeamStrategyController::TeamStrategyController( RobotTeam* _team, YAML::Node* n)
  : QObject(_team),
    team(_team),
    activeStrategy(new TeamStrategy(_team)) // set dummy strategy be default
{
    (void)n;
}




TeamStrategyController::~TeamStrategyController(){

}

void TeamStrategyController::setPause(bool pause){
    is_paused = pause;
}

void TeamStrategyController::resume(){
    is_paused = false;
}

void TeamStrategyController::runControlCycle(GameState* game_state)
{
    if(is_paused) return; // if the controller is paused, dont do anything

    // update controller state
    int new_state = game_state->hasRefereeCommandChanged() ?
                            getControllerState(game_state->getRefereeCommand()) :
                            getNextControllerState(controller_state, activeStrategy->getStatus());

    // clear flag
    game_state->clearRefereeCommandChanged();

    // if state changed, update strategy
    if(new_state!=controller_state){
        qInfo() << "updating strategy \n";
        controller_state = new_state;
        delete activeStrategy;
        for(Robot* r : team->getRobots()) r->clearBehavior();
        activeStrategy = loadStateStrategy(controller_state);
        activeStrategy->assignBehaviors();
        qInfo() << "Strategy changed to:" << activeStrategy->getName();
        emit team->strategyChanged(team->getID(), activeStrategy->getName());
    }


    // update strategy
    activeStrategy->runControlCycle();

    // perform robot behaviors
//    qInfo() << "updated";

}






TeamStrategyController* TeamStrategyController::loadController(RobotTeam* team, YAML::Node* c_node){
    qInfo() << "        STRATEGY_CONTROLLER";
    qInfo() << "            ID            -" <<  (*c_node)["ID"];
    auto id = (*c_node)["ID"].as<string>();

    if(id == "NORMAL_GAME"){
        return new SControllerNormalGame(team,c_node);
    } else if( id == "STRATEGY_TESTER"){
        return new SControllerStrategyTester(team,c_node);
    } else if ( id == "JOYSTICK" ) {
        return new SControllerJoystick(team,c_node);
    } else if(id == "none") {
        return new SControllerNone(team,c_node);
    } else {
        qCritical().nospace()
            << "ERROR : controller '" << id.c_str() << "' not recognized" <<endl
            << "\tHalting execution...";
        exit(-1);
    }


}

QString TeamStrategyController::getStrategyName(){
    return activeStrategy ? activeStrategy->getName() : "null";
}
