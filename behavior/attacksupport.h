#ifndef ATTACK_SUPPORT_H
#define ATTACK_SUPPORT_H

#include <stdlib.h>
#include "behavior/genericmovementbehavior.h"
#include "skill/kicktopoint.h"
#include "utilities/measurments.h"
#include "utilities/region.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "movement/gotoposition.h"

//Used in the TwoVOne strategy to control the robot that moves into position to receive passes
//TwoVOne assigns this behavior to the robot farther from the ball
//The complementary behavior is AttackMain

class AttackSupport : public GenericMovementBehavior
{
public:
    AttackSupport(const ParameterList& list);
	
    void perform(Robot *);
    enum {initial, final} state;
    Point previousBP;
};

#endif // ATTACK_SUPPORT_H
