#include "markbot.h"
#include "model/ball.h"
#include "model/team/team.h"
#include "model/robot/robot.h"
#include "model/game_state.h"

bool MarkBot::mark_status[10] = {false};

MarkBot::MarkBot(Robot* robot) : Behavior(robot), GenericMovementBehavior(robot)
{

}

bool MarkBot::perform()
{
    Point bp = *ball;
    float ang2ball = Measurements::angleBetween(robot, bp);

    // Determine whether we can mark a robot
    updateMark(robot);

    Point target = Point(-1500, 0);  // Idle Point

    // If an opponent can be marked, move between it and the ball
    if(marked_opp_id != -1)
    {
        Point opp_pos = *team->getOpponent(marked_opp_id);
        target = opp_pos + Measurements::unitVector(bp-opp_pos) * (2*ROBOT_RADIUS+50);
    }

    cmd.setTarget(target, ang2ball);
    GenericMovementBehavior::perform();
    return isFinished();
}

void MarkBot::updateMark(Robot* r)
{
    // For every opponent
    for(Robot* opp: team->getOpponents())
    {
        bool our_side = opp->x < 0;
        bool has_ball = game_state->getRobotWithBall()  &&
                        game_state->getRobotWithBall()->getId() == opp->getId();

        // Check if it is on our side
        if(our_side && !has_ball)
        {
            // Mark it if it's not already marked
            // and we are not already marking another opp
            if(mark_status[opp->getId()] == false && marked_opp_id == -1)
            {
                mark_status[opp->getId()] = true;
                marked_opp_id = opp->getId();
                break;
            }
        }
        // Unmark it if it's not on our side if it received the ball
        else
        {
            mark_status[opp->getId()] = false;

            if(marked_opp_id == opp->getId())
                marked_opp_id = -1;
        }
    }
}

bool MarkBot::isFinished()
{
    return false;
}

string MarkBot::getName() {
    return "MarkBot";
}
