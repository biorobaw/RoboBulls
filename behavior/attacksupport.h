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

/* Recycled behavior.
 * Old decription:
 * Used in the TwoVOne strategy to control the robot that moves into position to receive passes
 * TwoVOne assigns this behavior to the robot farther from the ball
 * The complementary behavior is AttackMain
 *
 * New description:
 * See old decription, but change TwoVOne to NormalGameStrategy. Also,
 * has been modernized to work with arbitrary IDs and can give more information
 * about itself.
 */

class AttackSupport : public GenericMovementBehavior
{
public:
    AttackSupport(const ParameterList& list);
    void perform(Robot *);
    Point getCurrentTarget();
private:
    Robot* main_attacker;
    enum {initial, final} state;
    Point previousBP;
    Point wp;
};

#endif // ATTACK_SUPPORT_H
