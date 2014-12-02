#include "mytestbehavior.h"
#include <iostream>
#include "model/robot.h"
#include "model/gamemodel.h"
#include "skill/driveball.h"
#include "math.h"
#include "behavior/genericmovementbehavior.h"
#include "skill/kick.h"

#if SIMULATED
    #define xx 1600
#else
    #define xx -1600
#endif

myTestBehavior::myTestBehavior()
    : GenericMovementBehavior()
{
    //GameModel *model = GameModel::getModel();
    Point kickPoint(1600, 0);
    //Point target = model->getPenaltyPoint();
//    this->TargetPoint = list.getParam<Point>("targetPoint");
    mySkill = new Skill::DriveBall(kickPoint, 0);
//    mySkill = new Skill::GoToPositionWithOrientation(kickPoint, 0);
//    mySkill = new Skill::Kick;
//    setMovementTargets(kickPoint, 3);


}

void myTestBehavior::perform(Robot * myRobot)
{
//    Point robotPoint = myRobot->getRobotPosition();
    //Testing closed loop control by using GoToPositionWithOrientation

    //GameModel *model = GameModel::getModel();

//    Point target = model->getBallPoint();

//    auto mySkill = Skill::GoToPositionWithOrientation(target, M_PI);
//    mySkill.perform(myRobot);

    //********************************************************************************

    //Testing drive ball

//    cout << model->getOpponentGoal()
//    cout << "performing" << endl;
//    cout << "angle\t" << myRobot->getOrientation()/M_PI*180 << endl;
//    cout << "position" << myRobot->getRobotPosition().toString() << endl;
    Point kickPoint(1600, 0);
//    Point kick2(-1300, 500);
//    move.setMovementTolerances(100, 10*(M_PI)/180);
//    move.setVelocityMultiplier(1);
//    move.recreate(kickPoint, 0, false);
//    move.perform(myRobot, Movement::Type::Default);
//    move.recreate(kick2, 0, false);
//    move.perform(model->getMyTeam().at(1), Movement::Type::Default);

//    mySkill->perform(myRobot);

    cout << "angle diff\t" << Measurments::angleDiff(0, myRobot->getOrientation())/M_PI*180 << endl;
    setMovementTargets(kickPoint, 0, false);
    GenericMovementBehavior::perform(myRobot);

}
