#include "stopbehavior.h"
#include "Model/robot.h"
#include  "Model/gamemodel.h"
#include "skill/closedloopcontrol.h"
#include <math.h>
#include "Utilities/point.h"
#include "Utilities/measurments.h"
#include "skill/gotopositionwithorientation.h"

#define RADIUS 1000
StopBehavior::StopBehavior()
{
//    Skill * activeSkill;
}

void StopBehavior::perform(Robot * myRobot)
{
    GameModel * gamemodel = GameModel::getModel();
    Point ballPoint = gamemodel->getBallPoint();
    Point robotPoint = myRobot->getRobotPosition();
    Point targetPoint ;
    float angle = Measurments::angleBetween(ballPoint, robotPoint);
//    targetPoint.x = (RADIUS*cos(angle))+ballPoint.x;
//    targetPoint.y = (RADIUS*sin(angle))+ballPoint.y;

    /*******************************
     *testing*/
    targetPoint.x = 1000;
    targetPoint.y = 1000;
    robotSkill = new GoToPositionWithOrientation(targetPoint, M_PI/2);
    //*********************************

//    robotSkill = new GoToPositionWithOrientation(targetPoint, Measurments::angleBetween(robotPoint, ballPoint));

    robotSkill->perform(myRobot);
}

Skill * StopBehavior::getSkill()
{
    return robotSkill;
}

