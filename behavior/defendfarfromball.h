#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H
#include "skill/kicktopointomni.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

/*! @brief The “goalie” behavior of the game.
 * @author Narges Ghaedi, James W
 * The robot defends the goal by positioning itself at the goal and moving
 * in between the goal and the ball facing the ball, wherever the ball may be.
 * When the ball becomes close, this behavior moves the robots out and
 * kicks the ball to the center of the field to remove it from the goalie box. */

class DefendFarFromBall : public GenericMovementBehavior
{
public:
    DefendFarFromBall();
    void perform(Robot*);
    static int goalieDist; //!< @brief Distance to goal needed for goalie to react

public:
    Skill::KickToPointOmni* KTPSkill = nullptr;
    bool wasNotPreviousScoreHazard;
    bool isKickingAwayBall;
    int  lastKickCounter;
    int  velChangeCounter;
    bool isOnSlowVelMode;
};

#endif // DEFENDFARFROMBALL_H
