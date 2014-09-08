#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior/genericmovementbehavior.h"

class KickToGoal : public GenericMovementBehavior
{
public:
    KickToGoal(const ParameterList& list);

    void perform(Robot * r);

    enum StateEnum {goingBehind, approaching, kicking};
    StateEnum state;
};

#endif // KICKTOGOAL_H
