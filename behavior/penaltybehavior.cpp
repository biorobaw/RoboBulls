#include "penaltybehavior.h"

PenaltyBehavior::PenaltyBehavior(const ParameterList& list)
{
    pb = moving;
}

/**
 * @brief PenaltyBehavior::perform Behavior that goes to the penalty point and kicks the ball once.
 * @param myRobot : The robot to perform the behavior on (usually myTeam[0])
 */
void PenaltyBehavior::perform(Robot * myRobot)
{
    GameModel *model = GameModel::getModel();

    Skill::Kick kick;
    Skill::Stop stop;
    Skill::GoToPosition go(model->getPenaltyPoint());

    switch(pb)
    {
    case moving:
        if(!(Measurments::isClose(myRobot->getRobotPosition(),model->getBallPoint(),100)))
        {
            go.perform(myRobot);
            cout <<"moving performed!"<<endl;
        }
        else
            pb = kicking;
        break;
    case kicking:
        kick.perform(myRobot);
        cout<<"kicking performed!"<<endl;
        pb = idling;
        break;
    case idling:
        stop.perform(myRobot);
        cout<<"idling performed!"<<endl;
        break;
    default: cout<<"Default case!"<<endl;

    }

}
