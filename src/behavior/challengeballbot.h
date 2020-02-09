#ifndef CHALLENGEBALLBOT_H
#define CHALLENGEBALLBOT_H

#include "src/behavior/genericmovementbehavior.h"
#include "src/model/robot.h"
#include "src/model/game_state.h"


/*! @file
 * ChallengeBallBot makes the robot stand a certain distance in front of the opponent with the ball.
 * If an opponent does not possess the ball, the robot stands still. Meant to block shots/passes.
 *
 * @todo Make it so that the robot tries to knock the ball away from the opponent.
 */

class ChallengeBallBot : public GenericMovementBehavior
{
public:
    ChallengeBallBot();
    void perform(Robot*) override;
};

#endif // CHALLENGEBALLBOT_H
