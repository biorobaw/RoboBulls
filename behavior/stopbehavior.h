#ifndef STOPBEHAVIOR_H
#define STOPBEHAVIOR_H

#include <iostream>
#include "behavior/behavior.h"

#include "skill/skill.h"

#include "utilities/measurments.h"
#include "skill/basic_movement.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "utilities/paramlist.h"

/*
 * Stop Behavior
 * All robots position themself one meter away from the ball
 * and face the ball
 * Narges Ghaedi
 */

using namespace std;

const float TARGET = 1000.0;
const float TOLERENCE = 100.0;

class GameModel;

class StopBehavior : public Behavior
{
public:
    StopBehavior(){};
    StopBehavior(const ParameterList &list);
    void perform(Robot *);
    Skill::Skill* getSkill();

private:
    Skill::Skill* robotSkill;
    GameModel *gamemodel;
};

#endif // STOPBEHAVIOR_H
