#include <iostream>
#include <list>
#include "include/config/team.h"
#include "utilities/measurements.h"
#include "behavior/goaliebehavior.h"
#include "behavior/refstop.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "stopstrategy.h"

StopStrategy::StopStrategy()
{
}

void StopStrategy::assignBeh()
{
    for(Robot* robot: gameModel->getMyTeam())
        robot->assignBeh<RefStop>();

    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<GoalieBehavior>();


}

bool StopStrategy::update()
{
    return false;
}
