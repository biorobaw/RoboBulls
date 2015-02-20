#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "behavior/ballreceiver.h"

/************************************************************************/
/* USER CONFIGURATION */

/* Defines the number of calls to `perform` in the MOVING state
 * to _wait_ in before going back to the initial state, in the case of
 * the ball having velocity again.
 * Use: Helps prevents Behavior from calculating false predictions by not
 *      letting the ball recieve its initial velocity from a source first.
 */
#define BALLRECEIVER_DEF_WAIT_COUNT 10

/* Defines the number of prediction detections that must be recorded before
 * they are all averaged to get the actual point to move to.
 * Use: Prevents the robot from recreating to a new prediction each call,
 *      and makes sure the final target is not a bogus prediction, by averaging.
 */
#define BALLRECEIVER_DEF_MAX_DETECT 10

/************************************************************************/

BallReceiver::BallReceiver()
    : state(INITIAL)
    , waitCount(0)
    , maxWaitCount(BALLRECEIVER_DEF_WAIT_COUNT)
    , maxDetections(BALLRECEIVER_DEF_MAX_DETECT)
    , hasReachedZero(false)
    , time(1.5)
    { }

void BallReceiver::perform(Robot *robot)
{
    switch(state)
    {
    case INITIAL:
        {
            waitCount = 0;
            hasReachedZero = false;
            detections.clear();
            target = Point(0,0);
            state = COLLECTING;
        }
    case COLLECTING:
        {
            Point bp = gameModel->getBallPoint();
            Point bv = gameModel->getBallVelocity() * 1250;
            Point ba = gameModel->getBallAcceleration();
            Point t  = bp + bv*time + ba*time*time*0.5;
            if(Comparisons::isPointInsideField(t))
                detections.push_back(t);
            if(detections.size() == maxDetections) {
                state = AVERAGING;
            }
        }
        break;
    case AVERAGING:
        {
            for(const Point& p : detections)
                target += p;
            target /= detections.size();
            state = MOVING;
        }
        break;
    case MOVING:
        {
            setMovementTargets(target);
            GenericMovementBehavior::perform(robot, Movement::Type::SharpTurns);

            if(gameModel->getBallSpeed() < 1) {
                hasReachedZero = true;
            }
            else if(hasReachedZero and (++waitCount > maxWaitCount)) {
                state = INITIAL;
            }
        }
        break;
    }
}
