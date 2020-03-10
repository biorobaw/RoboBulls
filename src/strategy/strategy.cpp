#include "strategy.h"
#include <iostream>
#include "model/game_state.h"

Strategy::Strategy(RobotTeam* _team) : team(_team) {

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



