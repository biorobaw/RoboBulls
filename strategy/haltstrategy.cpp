#include "haltstrategy.h"
#include "model/gamemodel.h"
#include "behavior/refstop.h"

void HaltStrategy::assignBeh()
{
    for(Robot* robot : gameModel->getMyTeam())
    {
        robot->setLF(0);
        robot->setLB(0);
        robot->setRF(0);
        robot->setRB(0);

        robot->setL(0);
        robot->setR(0);
        robot->setB(0);

        robot->setXVel(0);
        robot->setYVel(0);
        robot->setAngVel(0);
    }
}
