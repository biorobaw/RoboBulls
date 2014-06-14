#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior.h"
#include "utilities/paramlist.h"

#define CLOSE_TO_BALL 110

class KickToGoal : public Behavior
{
public:
    KickToGoal(const ParameterList& list);

    void perform(Robot * r);

    enum StateEnum {goingBehind, approaching, kicking};
    StateEnum state;

};

#endif // KICKTOGOAL_H
