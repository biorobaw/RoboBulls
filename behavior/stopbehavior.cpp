#include "stopbehavior.h"
#include "Model/robot.h"
#include  "Model/gamemodel.h"
#include "skill/closedloopcontrol.h"
#include <math.h>
#include "Utilities/point.h"
#include "Utilities/measurments.h"
#include "skill/gotopositionwithorientation.h"

#define RADIUS 500
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


//    //************************************************************************
//    //Testing shamsi's code
//    wheelvelocities wheelvelocity;
//    double robot_x = myRobot->getRobotPosition().x;
//    double robot_y = myRobot->getRobotPosition().y;
//    double robot_orientation = myRobot->getOrientation();
//    double ball_x = gamemodel->getBallPoint().x;
//    double ball_y = gamemodel->getBallPoint().y;

//    wheelvelocities wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation,0,0,-M_PI);
//    nxtbee->sendVels(wheelvelocity.left, wheelvelocity.right,robot->getID());
//    myRobot->setRightWheelVelocity(wheelvelocity.right);
//    myRobot->setLeftWheelVelocity(wheelvelocity.left);
//    //************************************************************************


//    cout << "Distance " << Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint()) << endl;

//    if (Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint()) < (TARGET - TOLERENCE))
//    {
//        cout<<"Calling GoBackward"<<endl;
////        wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation,0,0,-M_PI); //Cheking Shamsi's code
//        robotSkill = new GoBackward();
//    }
//    else if (Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint())> (TARGET - TOLERENCE) &&
//             Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint()) < (TARGET + TOLERENCE))
//    {
//        cout << "Stopping!" << endl;
////        wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation,0,0,-M_PI); //Cheking Shamsi's code
//        robotSkill = new Stop();
//    }
//    else
//    {
//        cout<<"Calling GoForward"<<endl;
////        wheelvelocity = closed_loop_control(robot_x, robot_y, robot_orientation,0,0,-M_PI); //Cheking Shamsi's code
//        robotSkill = new GoForward();
//    }


//    //************************************************************************
//    //Testing shamsi's code

//    myRobot->setRightWheelVelocity(wheelvelocity.right);
//    myRobot->setLeftWheelVelocity(wheelvelocity.left);
//    robotSkill = new GoForward();
//    //************************************************************************
    robotSkill = new GoToPositionWithOrientation(targetPoint);
    robotSkill->perform(myRobot);
    //myRobot->setCurrentBeh(StopBehavior);
}

Skill * StopBehavior::getSkill()
{
    return robotSkill;
}
