#include "strategy.h"
#include <iostream>
#include "model/gamemodel.h"

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
