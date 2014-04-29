#include <stdio.h>
#include "stopbehavior.h"
#include "Robot/robot.h"
#include "Model/gamemodel.h"
#include "Functionality/rotate.h"
#include "Model/gamemodel.h"
#include "Performance/gotoposition.h"
#include "include/util.h"

StopBehavior::StopBehavior()
{
    kickSent = false;
}

void StopBehavior::perform(Robot * myRobot)
{
    GameModel* model = GameModel::getModel();
    Point bpos = model->getBallPoint();
    robComm* nxt = robComm::getnxtbee();

    GoToPosition toPos(bpos);
    toPos.perform(myRobot);

    if(Measurments::distance(bpos, myRobot->getRobotPosition()) <= DIST_TOLERANCE+50) {
       // puts("Close!");
        if(!kickSent) {
            kickSent = true;
            nxt->sendKick(myRobot->getID());
        }
    } else {
        kickSent = false;
    }

    //myRobot->setCurrentBeh(StopBehavior);
}

Skill * StopBehavior::getSkill()
{
    return robotSkill;
}
