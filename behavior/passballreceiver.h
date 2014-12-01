#ifndef PASSBALLRECEIVER_H
#define PASSBALLRECEIVER_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "movement/gotoposition.h"
#include "utilities/point.h"
#include "model/robot.h"
#include "model/gamemodel.h"
#include "behavior/genericmovementbehavior.h"

/*
 * PassBallReceiver class:
 * This class can be used with passBallSender.
 * Once the sender passes the ball, an int count = 170
 * would start to count down.
 * The receiver robot waits for the ball until
 * the ball gets close to it or after the count becomes 0.
 * With differential robots, the kicking is not very
 * powerful. Hence, we use a counter. The counter starts
 * counting down after the sender passes the ball.
 * If the ball does not get close to receiver before count
 * become 0, the receiver moves toward the ball since
 * it assumes that the ball has stopped somewhere far from it.
 *
 * The receiver gets behind the ball and
 * shoots toward the goal
 * */

class PassBallReceiver : public GenericMovementBehavior
{
public:
    PassBallReceiver();
    void perform(Robot *);
    enum states {initial, moving, approaching, kicking, idling} state;
private:
    Point ballLastSeen;
    Point ballOrg;
    Point target;
    int count;
};

#endif // PASSBALLRECEIVER_H
