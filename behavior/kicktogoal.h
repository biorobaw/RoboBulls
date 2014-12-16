#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior/genericmovementbehavior.h"
#include "utilities/point.h"
#include "skill/kicktopoint.h"

class KickToGoal : public GenericMovementBehavior
{
public:
    KickToGoal();

    void perform(Robot * r);

    enum StateEnum {initial, kicking, stopping};
    StateEnum state;
private:
    Point target;
    bool sign;  //0 is positive and 1 is negative
    bool targetSign;
    Skill::KickToPoint *kickToPoint;
    Point ballOrig;
};

#endif // KICKTOGOAL_H
