#include "stopbehavior.h"

#include "model/robot.h"
#include "model/gamemodel.h"
#include "skill/gotopositionwithorientation.h"

#define RADIUS 1000

StopBehavior::StopBehavior(const ParameterList& list)
{
//    Skill * activeSkill;
}

void StopBehavior::perform(Robot * myRobot)
{
    GameModel* gamemodel = GameModel::getModel();
    Point ballPoint  = gamemodel->getBallPoint();
    Point robotPoint = myRobot->getRobotPosition();
    Point targetPoint;

    float angle = Measurments::angleBetween(ballPoint, robotPoint);
    targetPoint.x = (RADIUS*cos(angle))+ballPoint.x;
    targetPoint.y = (RADIUS*sin(angle))+ballPoint.y;


    robotSkill = new Skill::GoToPositionWithOrientation
//            (targetPoint, Measurments::angleBetween(robotPoint, ballPoint));
            (Point(0,0), 0);

    robotSkill->perform(myRobot);
}

Skill::Skill* StopBehavior::getSkill()
{
    return robotSkill;
}

