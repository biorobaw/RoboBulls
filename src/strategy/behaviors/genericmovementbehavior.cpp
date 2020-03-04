#include "genericmovementbehavior.h"
#include "robot/robot.h"

GenericMovementBehavior::GenericMovementBehavior(Point targetPose,
                                                 float targetAngle,
                                                 bool avoidObstacles,
                                                 bool avoidBall) :
    cmd(targetPose,targetAngle,avoidObstacles,avoidBall)
{

}

//Override Behavior perform
void GenericMovementBehavior::perform(Robot* robot)
{
    robot->getPilot()->goToPose(cmd);
    done = robot->getPilot()->finisedLastCommand();
}

/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/


bool GenericMovementBehavior::isFinished()
{
    return done;
}
