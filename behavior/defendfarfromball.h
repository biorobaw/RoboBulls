#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H
#include "skill/kicktopoint.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

/* Behavior that goes to the goal and sits between the ball.
 * Improvement: The behavior now kicks the ball away if close
 * then returns to sitting.
 */
class DefendFarFromBall : public GenericMovementBehavior
{
public:
    DefendFarFromBall(const ParameterList& list);
    void perform(Robot*);
public:
    Skill::KickToPoint* KTPSkill = nullptr;
    bool wasNotPreviousScoreHazard;
    bool isKickingAwayBall;
    int  lastKickCounter;
    int  velChangeCounter;
    bool isOnSlowVelMode;
};

#endif // DEFENDFARFROMBALL_H
