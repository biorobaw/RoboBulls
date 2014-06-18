#include <iostream>
#include "model/robot.h"
#include "skill/gotopositionwithorientation.h"
#include "stopbehavior.h"


StopBehavior::StopBehavior(const ParameterList& list)
{
    this->mTargetPoint = list.getParam<Point>("targetPoint");
    this->curBallPoint = list.getParam<Point>("ballPoint");
}


/* StopBehavior is simply a wrapper for
 * GoToPositionWithOrientation
 */
void StopBehavior::perform(Robot * myRobot)
{
    Point robotPoint = myRobot->getRobotPosition();

    auto mySkill = Skill::GoToPositionWithOrientation
        (mTargetPoint, Measurments::angleBetween(robotPoint, curBallPoint));

    mySkill.perform(myRobot);
}


