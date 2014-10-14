#include "behavior.h"

Behavior::Behavior(std::string name, Priority pri)
{
    this->name = name;
    this->priority = pri;
}

int Behavior::getPriority()
{
    return (int)priority;
}

std::string Behavior::toString()
{
    return name;
}
