#include "refstop.h"
#include "src/model/ball.h"

RefStop::RefStop()
{
}

void RefStop::perform(Robot * robot)
{
    Point bp = Ball::getPosition();

    if(Measurements::isClose(robot, bp, 600))
    {
        // Generate position vector away from ball
        Point reposition = bp + Measurements::unitVector(robot->getPosition() - bp) * 600;

        // If the vector is unreachable send the robot
        // to the center of the field. This should eventually
        // result in a legal position that is inside the field
        DefenceArea our_def(OPPONENT_DEFFENCE_AREA);
        DefenceArea opp_def(TEAM_DEFFENCE_AREA);

        bool reposition_invalid = Comparisons::isPointOutsideField(reposition)
                                          || (our_def.contains(reposition) && !robot->isGoalie())
                                          || opp_def.contains(reposition, 2*ROBOT_RADIUS);

        if(reposition_invalid)
            reposition = Point(0,0);

        setVelocityMultiplier(1);
        setMovementTargets(reposition, Measurements::angleBetween(robot->getPosition(), bp));

        finished  = false;
    }
    else
        setVelocityMultiplier(0);

    GenericMovementBehavior::perform(robot);

    finished = true;
}

bool RefStop::isFinished()
{
    return finished;
}
