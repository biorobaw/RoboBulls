#include "haltstrategy.h"
#include "model/gamemodel.h"
#include "behavior/refstop.h"

HaltStrategy::HaltStrategy(Team* _team) : Strategy(_team) {

}

void HaltStrategy::assignBeh()
{
    for(Robot* robot : team->getRobots())
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
