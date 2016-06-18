#include <iostream>
#include <list>
#include "include/config/team.h"
#include "utilities/measurements.h"
#include "behavior/goaliebehavior.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "stopstrategy.h"
#define STOPSTRAT_DEBUG 0

static bool reposition_invalid = false;

StopStrategy::StopStrategy()
{
}

void StopStrategy::assignBeh()
{
    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<GenericMovementBehavior>(gameModel->getMyGoal() + Point(ROBOT_RADIUS, 0),
                                                   Measurements::angleBetween(goalie, gameModel->getBallPoint()));

}

bool StopStrategy::update()
{
    Point bp = gameModel->getBallPoint();

    // Reposition every robot that is too close to the ball
    for(Robot* robot: gameModel->getMyTeam())
    {
        int id = robot->getID();

        if(Measurements::isClose(robot, bp, 600) && id != GOALIE_ID)
        {
            // Generate position vector away from ball
            Point reposition = bp + Measurements::unitVector(robot->getPosition() - bp) * 600;

            // If the vector is unreachable send the robot
            // to the center of the field. This should eventually
            // result in a legal position that is inside the field
            DefenceArea our_def(0);
            DefenceArea opp_def(1);

            reposition_invalid = Comparisons::isPointOutsideField(reposition)
                                              || our_def.contains(reposition)
                                              || opp_def.contains(reposition);

            if(negedge(reposition_invalid))
                need_assign[id] = true;

            if(reposition_invalid)
            {
                reposition = Point(0,0);
                need_assign[id] = true;
            }

            if(need_assign[id])
            {
                robot->clearCurrentBeh();
                robot->assignBeh<GenericMovementBehavior>(reposition, Measurements::angleBetween(robot, bp));
                need_assign[id] = false;
            }
        }
        else if(need_assign[id] && id != GOALIE_ID)
        {
            robot->clearCurrentBeh();
            robot->assignBeh<GenericMovementBehavior>(robot->getPosition(), Measurements::angleBetween(robot, bp));
            need_assign[id] = false;
        }
    }

    if (!Measurements::isClose(prev_point, bp, 25))
    {
        for(bool& b: need_assign)
            b = true;
        prev_point = bp;
    }

    return false;
}
