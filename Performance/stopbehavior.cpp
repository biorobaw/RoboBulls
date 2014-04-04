#include "stopbehavior.h"

StopBehavior::StopBehavior()
{
    Skill activeSkill;
}

void StopBehavior::perform(Robot * myRobot, Ball gameBall)
{
    cout<<"Stop behavior performing start!"<<endl;

    if (Measurments::distance(myRobot->getRobotPosition(),gameBall.getBallPosition()) < 10.0)
    {
        cout<<"Calling GoBackward"<<endl;
        robotSkill = new GoBackward();
    }
    else
    {
        cout<<"Calling GoForward"<<endl;
        robotSkill = new GoForward();
    }

    cout<<"Stop behavior performing!"<<endl;

    robotSkill->perform();
    //myRobot->setCurrentBeh(StopBehavior);
}

Skill StopBehavior::getSkill()
{
    return *robotSkill;
}
