#ifndef DRIVEBALLANDKICK_H
#define DRIVEBALLANDKICK_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/point.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotopositionwithorientation.h"

using namespace std;

class DriveBallAndKick : public Behavior
{
public:
    DriveBallAndKick();
    void perform(Robot *);

private:
    enum states {initial, driving, kicking, idling} state;
    Skill::Skill * skill;
    Point *behindBall;
    Point *ballPoint;
    Movement::GoToPosition move;
};

#endif // DRIVEBALLANDKICK_H
