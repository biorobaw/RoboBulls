#ifndef GOBACKWARD_H
#define GOBACKWARD_H

#include <iostream>
#include "skill/skill.h"
#include "communication/robcomm.h"

using namespace std;

class GoBackward:public Skill
{
public:
    GoBackward();
    void perform(Robot *);
};

#endif // GOBACKWARD_H
