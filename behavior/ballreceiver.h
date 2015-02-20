#ifndef BALL_RECEIVER_H
#define BALL_RECEIVER_H

#include "behavior/genericmovementbehavior.h"

/*
 * TEMPORARY FILE 
 * This is a temporary file, but the functionality of this file
 * will be later integrated in the codebase under a finalized name.
 */

/* `BallReceiver` is a predictive "go to the ball" behavior.
 * The velocity and acceleration are used to predict the ball's position,
 * and, while the behavior is active, the robot will move to the ball's
 * future position, to "intercept" it.
 * It works by recording a number of predictions, and moving to the average
 * of the collection. The robot will stop once reaching the position, and only
 * move again once the ball has more velocity.
 */

class BallReceiver : public GenericMovementBehavior
{
public:
    BallReceiver();
    void perform(Robot* robot) override;

private:
    enum { INITIAL, COLLECTING, AVERAGING, MOVING } state;

    std::vector<Point> detections;
    unsigned waitCount;
    unsigned maxWaitCount;
    unsigned maxDetections;
    bool     hasReachedZero;
    float    time;
    Point    target;
};

#endif
