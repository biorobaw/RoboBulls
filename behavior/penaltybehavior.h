#ifndef PENALTYBEHAVIOR_H
#define PENALTYBEHAVIOR_H

#include <iostream>
#include "behavior/behavior.h"

#include "skill/skill.h"
#include "utilities/measurments.h"
#include "skill/basic_movement.h"
#include "skill/gotoposition.h"
#include "skill/rotate.h"
#include "skill/kick.h"
#include "model/gamemodel.h"
#include "utilities/paramlist.h"


class PenaltyBehavior : public Behavior
{
public:
    PenaltyBehavior(const ParameterList& list);
    void perform(Robot *);

    enum penaltyBehavior_t {moving, kicking, idling} pb;
};

#endif // PENALTYBEHAVIOR_H
