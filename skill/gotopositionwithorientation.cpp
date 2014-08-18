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

    bool GoToPositionWithOrientation::
    perform(Robot* robot)
    {
//        #if TRACE
//            cout <<"Performing Skill::GoToPosition" << endl;
//        #endif

        RobComm *nxtbee = RobComm::getRobComm();

        wheelvelocities wheelvelocity =
            control.closed_loop_control(robot, targetPosition.x, targetPosition.y, goalOrientation);

        //float left_wheel_velocity  = wheelvelocity.left;
        //float right_wheel_velocity = wheelvelocity.right;

        //nxtbee->sendVels(left_wheel_velocity, right_wheel_velocity, robot->getID());

        robot->setL(wheelvelocity.left);
        robot->setR(wheelvelocity.right);
		
		
        if(Measurments::isClose(targetPosition, robot->getRobotPosition(), 250)) {
			return true;
		} else {
			return false;
		}
    }
}
