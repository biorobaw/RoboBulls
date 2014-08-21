#include "mytestbehavior.h"
#include <iostream>
#include "model/robot.h"
#include "skill/gotopositionwithorientation.h"
#include "model/gamemodel.h"
#include "skill/driveball.h"
#include "math.h"

#if SIMULATED
    #define xx 1600
#else
    #define xx -1600
#endif

myTestBehavior::myTestBehavior(const ParameterList& list)
{
    GameModel *model = GameModel::getModel();
    Point kickPoint(1600, 0);
    Point target = model->getPenaltyPoint();
//    this->TargetPoint = list.getParam<Point>("targetPoint");
    mySkill = new Skill::DriveBall(kickPoint, 0);
//    mySkill = new Skill::GoToPositionWithOrientation(kickPoint, 0);
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

//    cout << model->getOpponentGoal()
//    cout << "performing" << endl;
//    cout << "angle\t" << myRobot->getOrientation() << endl;
//    cout << "position" << myRobot->getRobotPosition().x << "\t" << myRobot->getRobotPosition().y << endl;

    mySkill->perform(myRobot);
}
