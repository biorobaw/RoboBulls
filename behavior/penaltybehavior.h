#ifndef PENALTYBEHAVIOR_H
#define PENALTYBEHAVIOR_H

#include <iostream>
#include "behavior/behavior.h"

#include "skill/skill.h"
#include "Utilities/measurments.h"
#include "skill/goforward.h"
#include "skill/stop.h"
#include "skill/rotate.h"
#include "skill/gotoposition.h"
#include "skill/kick.h"
#include "Model/gamemodel.h"


using namespace std;

class Robot;
class GameModel;

class PenaltyBehavior:public Behavior
{
public:
    PenaltyBehavior();
    void perform(Robot *);
//    Skill * getSkill();

    enum penaltyBehavior_t {moving, kicking, idling} pb;

//private:
//    Skill * robotSkill;

};

#endif // PENALTYBEHAVIOR_H
