#ifndef KICKTOPOINT_H
#define KICKTOPOINT_H

#include "skill/skill.h"
#include "model/gamemodel.h"
#include "movement/move.h"

namespace Skill{

class KickToPoint : public Skill
{
public:
    KickToPoint(Point target);
    KickToPoint(Point target,float targetToleranceRadians);
   ~KickToPoint();
    bool perform(Robot *);
private:
#if SIMULATED
    const int dist_position = ROBOT_RADIUS*3;
    const int dist_kick = 115;
#else
    const int dist_position = ROBOT_RADIUS*3;
    const int dist_kick = 220;
#endif
    Point target;
    float target_tolerance = ROT_TOLERANCE;
    Skill * skill;
    Movement::Move* move_skill;
    enum {positioning, kick} state;
};

} //namespace Skill

#endif // KICKTOPOINT_H
