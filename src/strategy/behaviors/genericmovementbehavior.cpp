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
void GenericMovementBehavior::perform()
{
//    std::cout << "generic movement - robot " <<robot->getID() <<std::endl;
    robot->goToPose(cmd);
    done = robot->completedGoToPoseCmd();
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
