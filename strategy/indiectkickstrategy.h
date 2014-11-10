#ifndef INDIECTKICKSTRATEGY_H
#define INDIECTKICKSTRATEGY_H

#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "model/robot.h"

/*Indirect Kick
 * This strategy is used when the player
 * is not allowed to score a goal directly
 * and has to pass it to another team player.
 *
 * This is a simple form of indirect kick which
 * uses PassBallSender and PassBallReceiver.
 *
 * PassBallSender will be the player that is closest
 * to the ball. It finds a point in front of one of the
 * players that has the least amount of apponent players
 * next to it and passes the ball to that point.
 *
 * PassBallReceiver is the player that has been chosen
 * by PassBallSender. It waits untill the ball gets close
 * to it or untill the ball stops rolling even if it was
 * still far from the receiver. Then, the receiver robot
 *  moves toward the ball.
 *
 * We need atleast two robots on the field to perform
 * indirectkick since indirectkick requires passing
 * skill. There would be segmentation fault if we have less
 * that two robots on the field
 *
 * Ideally, we should use the same robot that has been
 * found by PassBallSender. For now, I used the same
 * mechanism that PassBallSender uses in this strategy
 * to find the robot that sender will send the ball to
 * */


class IndiectKickStrategy : public Strategy
{
public:
    IndiectKickStrategy();
    void assignBeh();
};

#endif // INDIECTKICKSTRATEGY_H
