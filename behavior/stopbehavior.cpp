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
    targetPoint.x = (RADIUS*cos(angle))+ballPoint.x;
    targetPoint.y = (RADIUS*sin(angle))+ballPoint.y;

    robotSkill = new GoToPositionWithOrientation(targetPoint);
    robotSkill->perform(myRobot);
}

Skill * StopBehavior::getSkill()
{
    return robotSkill;
}
