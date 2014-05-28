#ifndef STOPBEHAVIOR_H
#define STOPBEHAVIOR_H

#include <iostream>
#include "behavior/behavior.h"

#include "skill/skill.h"
#include "Utilities/measurments.h"
#include "skill/gobackward.h"
#include "skill/goforward.h"
#include "skill/stop.h"
#include "skill/gotoposition.h"

/*
 * Stop Behavior
 * All robots position themself one meter away from the ball
 * and face the ball
 * Narges Ghaedi
 */

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
  GameModel *gamemodel;
};

#endif // STOPBEHAVIOR_H
