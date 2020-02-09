#include "strategy.h"
#include <iostream>
#include "src/model/game_state.h"

Strategy::Strategy(Team* _team) : team(_team) {

}

Strategy::~Strategy() 
{
}

char Strategy::getNextStrategy()
{
    return '\0';
}

bool Strategy::update()
{
    return false;
}

Team* Strategy::getTeam(){
    return team;
}
