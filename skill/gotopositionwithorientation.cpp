#include <math.h>
#include "gotopositionwithorientation.h"
#include "utilities/measurments.h"
#include "communication/robcomm.h"
#include "include/util.h"
#include "skill/differential_control/closedloopcontrol.h"
#include "model/gamemodel.h"


namespace Skill {

    GoToPositionWithOrientation::GoToPositionWithOrientation(Point target, double goalOrientation)
    {
        targetPosition = target;
        this->goalOrientation = goalOrientation;
    }

    GoToPositionWithOrientation::GoToPositionWithOrientation(float tx, float ty, double goalOrientation)
    {
        targetPosition = Point(tx, ty);
        this->goalOrientation = goalOrientation;
    }

    void GoToPositionWithOrientation::setVelocityMultiplier(double multipier)
    {
        this->control.setVelMultiplier(multipier);
    }

    bool GoToPositionWithOrientation::perform(Robot* robot)
    {
        #if TRACE
            cout <<"Performing Skill::GoToPosition" << endl;
        #endif

        wheelvelocities wheelvelocity =
            control.closed_loop_control(robot, targetPosition.x, targetPosition.y, goalOrientation);

        robot->setL(wheelvelocity.left);
        robot->setR(wheelvelocity.right);
		
        #if SIMULATED
            int dist_tolerance = DIST_TOLERANCE;
        #else
            int dist_tolerance = DIST_TOLERANCE;
        #endif

        if(Measurments::isClose(targetPosition, robot->getRobotPosition(), dist_tolerance)
           && abs(Measurments::angleDiff(robot->getOrientation(),goalOrientation))<ROT_TOLERANCE) {
			return true;
		} else {
			return false;
		}
    }
}
