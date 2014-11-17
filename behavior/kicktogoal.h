#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior/genericmovementbehavior.h"
#include "utilities/point.h"

class KickToGoal : public GenericMovementBehavior
{
public:
    KickToGoal(const ParameterList& list);

    void perform(Robot * r);

    enum StateEnum {goingBehind, approaching, kicking, stopping};
    StateEnum state;
private:
    Point target;
};

#endif // KICKTOGOAL_H
