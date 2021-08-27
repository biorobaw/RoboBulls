#include "refstop.h"
#include "model/ball.h"
#include "model/robot/robot.h"
#include "model/game_state.h"
#include "model/team/team.h"

RefStop::RefStop(Robot* robot) : Behavior(robot), GenericMovementBehavior(robot)
{
}

bool RefStop::perform()
{
    Point bp = *ball;

    if(Measurements::isClose(robot, bp, 600))
    {
        // Generate position vector away from ball
        Point reposition = bp + Measurements::unitVector(*robot - bp) * 600;

        // If the vector is unreachable send the robot
        // to the center of the field. This should eventually
        // result in a legal position that is inside the field
        DefenceArea our_def(OPPONENT_SIDE);
        DefenceArea opp_def(OUR_SIDE);

        bool reposition_invalid = Comparisons::isPointOutsideField(reposition)
                                          || (our_def.contains(reposition) && !robot->isGoalie())
                                          || opp_def.contains(reposition, 2*ROBOT_RADIUS);

        if(reposition_invalid)
            reposition = Point(0,0);

        cmd.velocity_multiplier = 1;
        cmd.setTarget(reposition, Measurements::angleBetween(*robot, bp));

        finished  = false;
    }
    else
        cmd.velocity_multiplier = 0;

    if (GenericMovementBehavior::perform())
        finished = true;
    return isFinished();
}

bool RefStop::isFinished()
{
    return finished;
}

string RefStop::getName() {
    return "RefStop";
}
