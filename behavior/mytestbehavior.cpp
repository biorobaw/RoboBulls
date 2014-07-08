#include "mytestbehavior.h"
#include <iostream>
#include "model/robot.h"
#include "skill/gotopositionwithorientation.h"
#include "model/gamemodel.h"

myTestBehavior::myTestBehavior(const ParameterList& list)
{
//    this->TargetPoint = list.getParam<Point>("targetPoint");
}

void myTestBehavior::perform(Robot * myRobot)
{
//    Point robotPoint = myRobot->getRobotPosition();
    //Testing closed loop control by using GoToPositionWithOrientation

    GameModel *model = GameModel::getModel();

    Point target = model->getBallPoint();

    auto mySkill = Skill::GoToPositionWithOrientation(target, M_PI);
    mySkill.perform(myRobot);
}
