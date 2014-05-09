#ifndef STOP_H
#define STOP_H


#include <iostream>
#include "skill/skill.h"
#include "communication/robcomm.h"

using namespace std;

class Stop:public Skill
{
public:
    Stop();
    void perform(Robot *);
};

#endif // STOP_H
