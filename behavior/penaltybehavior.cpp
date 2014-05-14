#include "penaltybehavior.h"

PenaltyBehavior::PenaltyBehavior()
{
    pb = moving;
//    cout<<"pb is in constructor : "<<pb<<endl;
}

/**
 * @brief PenaltyBehavior::perform Behavior that goes to the penalty point and kicks the ball once.
 * @param myRobot
 */
void PenaltyBehavior::perform(Robot * myRobot)
{
    cout << "Penalty behavior performing"<<endl;

//    robComm *nxtbee = robComm::getnxtbee();

    GameModel *model = GameModel::getModel();

    Skill *go = new GoToPosition(model->getPenaltyPoint().x, model->getPenaltyPoint().y);
    Skill *kick = new Kick();
    Skill *stop = new Stop();

    switch(pb)
    {
    case moving:
//        cout<<"pb is : "<<pb<<endl;
        if(!(Measurments::isClose(myRobot->getRobotPosition(),model->getPenaltyPoint(),250)))
        {
            go->perform(myRobot);
            cout <<"moving performed!"<<endl;
        }
        else
            pb = kicking;
//        cout<<"pb is : "<<pb<<endl;
        break;
    case kicking:
//        cout<<"I'm here!"<<endl;
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

//    robotSkill->perform(myRobot);

//    if(Measurments::isClose(myRobot->getRobotPosition(),model->getPenaltyPoint(),300))
//        kicking->perform(myRobot);

//    if (myRobot->getRobotPosition()== model->getPenaltyPoint())
//        kicking->perform(myRobot);

//    nxtbee->sendKick(myRobot->getID());

}

//Skill * PenaltyBehavior::getSkill()
//{
//    return robotSkill;
//}
