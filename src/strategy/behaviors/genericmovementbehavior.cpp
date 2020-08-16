#include "genericmovementbehavior.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"

GenericMovementBehavior::GenericMovementBehavior(Robot* robot,
                                                 Point targetPose,
                                                 float targetAngle,
                                                 bool avoidObstacles,
                                                 bool avoidBall) :
    Behavior(robot),
    cmd(targetPose,targetAngle,avoidObstacles,avoidBall)
{

}

//Override Behavior perform
bool GenericMovementBehavior::perform()
{
//    std::cout << "generic movement - robot " <<robot->getID() <<std::endl;
    robot->goToPose(cmd);
    done = robot->completedGoToPoseCmd();
    return isFinished();
}

/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/


bool GenericMovementBehavior::isFinished()
{
    return done;
}

string GenericMovementBehavior::getName() {
    return "GenericMovement";
}
