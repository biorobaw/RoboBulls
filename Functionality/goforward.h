#ifndef GOFORWARD_H
#define GOFORWARD_H

#include <iostream>
#include "Performance/skill.h"

using namespace std;

class GoForward: public Skill
{
public:
    GoForward();
    void perform();
};

#endif // GOFORWARD_H
