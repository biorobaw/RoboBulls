#include "penaltybehavior.h"

PenaltyBehavior::PenaltyBehavior()
{
    pb = moving;
}

/**
 * @brief PenaltyBehavior::perform Behavior that goes to the penalty point and kicks the ball once.
 * @param myRobot
 */
void PenaltyBehavior::perform(Robot * myRobot)
{

    GameModel *model = GameModel::getModel();

    Skill *go = new GoToPosition(model->getPenaltyPoint().x, model->getPenaltyPoint().y);
    Skill *kick = new Kick();
    Skill *stop = new Stop();

    switch(pb)
    {
    case moving:
        if(!(Measurments::isClose(myRobot->getRobotPosition(),model->getPenaltyPoint(),250)))
        {
            go->perform(myRobot);
            cout <<"moving performed!"<<endl;
        }
        else
            pb = kicking;
        break;
    case kicking:
        kick->perform(myRobot);
        cout<<"kicking performed!"<<endl;
        pb = idling;
        break;
    case idling:
        stop->perform(myRobot);
        cout<<"idling performed!"<<endl;
        break;
    default: cout<<"Default case!"<<endl;

    }

}

//Skill * PenaltyBehavior::getSkill()
//{
//    return robotSkill;
//}
