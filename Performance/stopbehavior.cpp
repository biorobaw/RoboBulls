#include "stopbehavior.h"
#include "Robot/robot.h"
#include  "Model/gamemodel.h"

StopBehavior::StopBehavior()
{
//    Skill * activeSkill;
}

void StopBehavior::perform(Robot * myRobot)
{
    cout<<"Stop behavior performing start!"<<endl;

    GameModel * gamemodel = GameModel::getModel();


    cout << "Distance " << Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint()) << endl;

    if (Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint()) < 1000.0)
    {
        cout<<"Calling GoBackward"<<endl;
        robotSkill = new GoBackward();
    }
    else if (Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint())> 1000.0 &&
             Measurments::distance(myRobot->getRobotPosition(),gamemodel->getBallPoint()) < 1200.0)
    {
        cout << "Stopping!" << endl;
        robotSkill = new Stop();
    }
    else
    {
        cout<<"Calling GoForward"<<endl;
        robotSkill = new GoForward();
    }


    robotSkill->perform(myRobot);
    //myRobot->setCurrentBeh(StopBehavior);
}

Skill * StopBehavior::getSkill()
{
    return robotSkill;
}
