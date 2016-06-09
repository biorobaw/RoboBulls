#ifndef CHALLENGEBALLBOT_H
#define CHALLENGEBALLBOT_H

#include "behavior/genericmovementbehavior.h"
#include "model/robot.h"
#include "model/gamemodel.h"


/*
 * A behavior that gets in front
 * of the enemy robot with the ball.
 */

class ChallengeBallBot : public GenericMovementBehavior
{
public:
    ChallengeBallBot();
    void perform(Robot*) override;
};

#endif // CHALLENGEBALLBOT_H
