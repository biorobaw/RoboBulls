#include "penaltybehavior.h"

PenaltyBehavior::PenaltyBehavior()
{
}

void PenaltyBehavior::perform(Robot * myRobot)
{
    cout << "Penalty behavior performing"<<endl;

//    robComm *nxtbee = robComm::getnxtbee();

    GameModel *model = GameModel::getModel();

    robotSkill = new GoToPosition(model->getPenaltyPoint().x, model->getPenaltyPoint().y);

//    float u = model->getPenaltyPoint().x;

    robotSkill->perform(myRobot);

//    nxtbee->sendKick(myRobot->getID());

}

Skill * PenaltyBehavior::getSkill()
{
    return robotSkill;
}
