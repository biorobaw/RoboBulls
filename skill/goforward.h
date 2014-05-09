#ifndef GOFORWARD_H
#define GOFORWARD_H

#include <iostream>
#include "skill/skill.h"

using namespace std;

class GoForward: public Skill
{
public:
    GoForward();
    void perform(Robot *robot);
};

#endif // GOFORWARD_H
