#include "scontroller_none.h"

SControllerNone::SControllerNone(RobotTeam* team, YAML::Node* node) :
    StrategyController(team,node)
{

}


int SControllerNone::getControllerState(Referee_Command command){
    (void) command;
    return UNINITIALIZED_STATE;
}
int SControllerNone::getNextControllerState(int current_state,int strategy_status){
    (void) current_state;
    (void) strategy_status;
    return UNINITIALIZED_STATE;
}
Strategy* SControllerNone::loadStateStrategy(int state){
    (void) state;
    return nullptr;
}
