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
//    cout<<"Stop behavior performing start!"<<endl;

    GameModel * gamemodel = GameModel::getModel();
    Point ballPoint = gamemodel->getBallPoint();
    Point robotPoint = myRobot->getRobotPosition();
//    float ball_x = ballPoint.x;
//    float ball_y = ballPoint.y;
    Point targetPoint ;
    float angle = Measurments::angleBetween(ballPoint, robotPoint);
//    float distance = Measurments::distance(targetPoint, gamemodel->getBallPoint());
    targetPoint.x = (RADIUS*cos(angle))+ballPoint.x;
    targetPoint.y = (RADIUS*sin(angle))+ballPoint.y;

    cout<<"Target point \t x: "<<targetPoint.x<<"\t y: "<<targetPoint.y<<endl;


    robotSkill = new GoToPositionWithOrientation(targetPoint, Measurments::angleBetween(robotPoint, ballPoint));
//    robotSkill = new GoToPositionWithOrientation(Point(0,0),M_PI/2);
    robotSkill->perform(myRobot);
    //myRobot->setCurrentBeh(StopBehavior);
}

Skill * StopBehavior::getSkill()
{
    return robotSkill;
}

