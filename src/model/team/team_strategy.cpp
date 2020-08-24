#include "team_strategy.h"
#include <iostream>
#include "model/game_state.h"
#include "model/team/team.h"

TeamStrategy::TeamStrategy(RobotTeam* _team) : // note the initialization order is important
    team(_team), game_state(team->getGameState()), ball(game_state->getBall()) {

}

TeamStrategy::~TeamStrategy()
{
}

void TeamStrategy::runControlCycle()
{

}

int TeamStrategy::getStatus(){
    return STATUS_RUNNING;
}



QString TeamStrategy::getName(){
    return "none";
}

void TeamStrategy::assignBehaviors(){

}
