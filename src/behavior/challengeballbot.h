#ifndef CHALLENGEBALLBOT_H
#define CHALLENGEBALLBOT_H

#include "behavior/genericmovementbehavior.h"
#include "robot/robot.h"
#include "model/game_state.h"


/*! @file
 * ChallengeBallBot makes the robot stand a certain distance in front of the opponent with the ball.
 * If an opponent does not possess the ball, the robot stands still. Meant to block shots/passes.
 *
 * @todo Make it so that the robot tries to knock the ball away from the opponent.
 */

class ChallengeBallBot : public Behavior//public GenericMovementBehavior
{
public:
    ChallengeBallBot();
    void perform(Robot*) override;

    bool isFinished() override;

private:
    bool done = false;

};

#endif // CHALLENGEBALLBOT_H
