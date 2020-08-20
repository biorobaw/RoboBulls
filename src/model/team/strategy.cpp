#include "strategy.h"
#include <iostream>
#include "model/game_state.h"
#include "model/team/team.h"

Strategy::Strategy(RobotTeam* _team) : // note the initialization order is important
    team(_team), game_state(team->getGameState()), ball(game_state->getBall()) {

}

Strategy::~Strategy() 
{
}

void Strategy::update()
{

}

int Strategy::getStatus(){
    return STATUS_RUNNING;
}



QString Strategy::getName(){
    return "none";
}

void Strategy::assignBehaviors(){

}
