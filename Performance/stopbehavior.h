#ifndef STOPBEHAVIOR_H
#define STOPBEHAVIOR_H

#include <iostream>
#include "Performance/behavior.h"

#include "Performance/skill.h"
#include "Measure/measurments.h"
#include "Functionality/gobackward.h"
#include "Functionality/goforward.h"
#include "Functionality/stop.h"


using namespace std;

class Robot;
class GameModel;

class StopBehavior : public Behavior
{
public:
    StopBehavior();
    void perform(Robot *);
    Skill * getSkill();

private:
    Skill * robotSkill;

//protected:
//    GameModel *gamemodel;
};

#endif // STOPBEHAVIOR_H
