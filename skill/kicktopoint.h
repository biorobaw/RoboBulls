#ifndef KICKTOPOINT_H
#define KICKTOPOINT_H

#include "skill/skill.h"
#include "model/gamemodel.h"

namespace Skill{

class KickToPoint
{
public:
    KickToPoint(Point target);
    KickToPoint(Point target,float targetToleranceRadians);
    bool perform(Robot *);
private:
#if SIMULATED
    const int dist_position = 115;
    const int dist_kick = 115;
#else
    const int dist_position = 50;
    const int dist_kick = 220;
#endif
    Point target;
    float target_tolerance = ROT_TOLERANCE;
    Skill * skill;
    enum {positioning, kick} state;
};

} //namespace Skill

#endif // KICKTOPOINT_H
