#include "behavior.h"

Behavior::Behavior(Robot* robot) : robot(robot){

}

Behavior::~Behavior()
{

}

bool Behavior::isFinished()
{
    return true;
}
