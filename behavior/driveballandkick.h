#ifndef DRIVEBALLANDKICK_H
#define DRIVEBALLANDKICK_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/paramlist.h"
#include "utilities/point.h"

using namespace std;

class DriveBallAndKick : public Behavior
{
public:
    DriveBallAndKick(const ParameterList& list);
    void perform(Robot *);

private:
    enum states {initial, driving, kicking, idling, finalOrientationFixing} state;
    Skill::Skill * skill;
    Point *behindBall;
    Point *ballPoint;
};

#endif // DRIVEBALLANDKICK_H
