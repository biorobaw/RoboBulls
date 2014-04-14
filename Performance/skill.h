#ifndef SKILL_H
#define SKILL_H

#include <iostream>
#include "behavior.h"


using namespace std;

class Skill
{
public:
    Skill();
    virtual void perform(Robot * robot) = 0;
};

#endif // SKILL_H
