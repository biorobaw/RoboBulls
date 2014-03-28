#ifndef STOPBEHAVIOR_H
#define STOPBEHAVIOR_H

#include <iostream>
#include "Performance/behavior.h"
#include "Robot/robot.h"
#include "Robot/ball.h"
#include "Performance/skill.h"
#include "Measure/measurments.h"
#include "Functionality/gobackward.h"
#include "Functionality/goforward.h"

using namespace std;

class StopBehavior: public Behavior
{
public:
    StopBehavior();
    void perform(Robot *, Ball);
    Skill getSkill();

private:
    Skill *robotSkill;
};

#endif // STOPBEHAVIOR_H
