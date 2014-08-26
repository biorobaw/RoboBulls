#include "mytestbehavior.h"
#include <iostream>
#include "model/robot.h"
#include "skill/gotopositionwithorientation.h"
#include "model/gamemodel.h"
#include "skill/driveball.h"
#include "math.h"

myTestBehavior::myTestBehavior(const ParameterList& list)
{
    UNUSED_PARAM(list);
    GameModel *model = GameModel::getModel();
    Point target = model->getPenaltyPoint();
//    this->TargetPoint = list.getParam<Point>("targetPoint");
    mySkill = new Skill::DriveBall(target, 0);
}

void myTestBehavior::perform(Robot * myRobot)
{
//    Point robotPoint = myRobot->getRobotPosition();
    //Testing closed loop control by using GoToPositionWithOrientation

//    GameModel *model = GameModel::getModel();

//    Point target = model->getBallPoint();

//    auto mySkill = Skill::GoToPositionWithOrientation(target, M_PI);
//    mySkill.perform(myRobot);

    //********************************************************************************

    //Testing drive ball

    mySkill->perform(myRobot);
}
