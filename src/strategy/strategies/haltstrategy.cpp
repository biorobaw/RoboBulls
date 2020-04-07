#include "haltstrategy.h"
#include "model/game_state.h"
#include "../behaviors/refstop.h"
#include "model/team.h"

HaltStrategy::HaltStrategy(RobotTeam* _team) : Strategy(_team) {

}

void HaltStrategy::assignBehaviors()
{
    for(Robot* robot : team->getRobots())
    {
        robot->getPilot()->setManualVelocity(Point(0,0),0);

    }
}
