#include "stopbehavior.h"
#include "model/robot.h"
#include  "model/gamemodel.h"

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

    cout<<"Target point \t x: "<<targetPoint.x<<"\t y: "<<targetPoint.y<<endl;

    //robotSkill = new GoToPositionWithOrientation(targetPoint);
    //robotSkill->perform(myRobot);
}

Skill::Skill* StopBehavior::getSkill()
{
    return robotSkill;
}
