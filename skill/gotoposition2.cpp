#include <stdio.h>
#include "model/gamemodel.h"
#include "utilities/measurments.h"
#include "skill/rotate.h"
#include "skill/basic_movement.h"
#include "skill/gotoposition.h"
#include "skill/gotoposition2.h"
#include "include/util.h"

namespace Skill {

GoToPosition2::GoToPosition2(float x, float y)
{
    mTargetPoint = Point(x, y);
    state = rotating;
}

GoToPosition2::GoToPosition2(Point target)
{
    mTargetPoint = target;
    state = rotating;
    std::cout << "GOTOPOS2: " << target.toString() << std::endl;
}

void GoToPosition2::perform(Robot* robot)
{
    mTargetPoint = GameModel::getModel()->getBallPoint();

    float currentAng = robot->getOrientation();
    float targetAng = Measurments::angleBetween(
                robot->getRobotPosition(), mTargetPoint);
    float angleDiff = angle_mod(targetAng - currentAng);

    Rotate rot(targetAng, true);
    GoToPosition go(mTargetPoint);
    Stop stop;

    switch(state)
    {
    case rotating:
        rot.perform(robot);
        if(fabs(angleDiff) <= 1.5*ROT_TOLERANCE) {
            puts("Making state moving");
            state = moving;
        }
        break;
    case moving:
        go.perform(robot);
        if(Measurments::isClose(robot->getRobotPosition(), mTargetPoint, DIST_TOLERANCE)) {
            puts("Making state idling");
            state = idling;
        }
        break;
    case idling:
        stop.perform(robot);
        if(fabs(angleDiff) > 2*ROT_TOLERANCE) {
            puts("Making state rotating");
            state = rotating;
        }
        break;
    }
}

}
