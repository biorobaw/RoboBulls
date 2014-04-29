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

const float TARGET = 1000.0;
const float TOLERENCE = 100.0;

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

    bool    kickSent;
    int     direction;
    float   targetAngle;

//protected:
//    GameModel *gamemodel;
};

#endif // STOPBEHAVIOR_H
