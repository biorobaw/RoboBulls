#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H
#include "skill/kicktopointomni.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

/* Behavior that goes to the goal and sits between the ball.
 * Improvement: The behavior now kicks the ball away if close
 * then returns to sitting.
 */
class DefendFarFromBall : public GenericMovementBehavior
{
public:
    DefendFarFromBall();
    void perform(Robot*);
public:
    Skill::KickToPointOmni* KTPSkill = nullptr;
    bool wasNotPreviousScoreHazard;
    bool isKickingAwayBall;
    int  lastKickCounter;
    int  velChangeCounter;
    bool isOnSlowVelMode;
};

#endif // DEFENDFARFROMBALL_H
