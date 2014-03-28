#ifndef GOBACKWARD_H
#define GOBACKWARD_H

#include <iostream>
#include "Performance/skill.h"

using namespace std;

class GoBackward:public Skill
{
public:
    GoBackward();
    void perform();
};

#endif // GOBACKWARD_H
