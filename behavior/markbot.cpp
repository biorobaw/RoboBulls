#include "markbot.h"

bool MarkBot::mark_status[10] = {false};

MarkBot::MarkBot()
{

}

void MarkBot::perform(Robot * robot)
{
    Point bp = gameModel->getBallPoint();
    float ang2ball = Measurements::angleBetween(robot, bp);

    // Determine whether we can mark a robot
    updateMark();

    Point target = Point(-1500, 0);  // Idle Point

    // If an opponent can be marked, move between it and the ball
    if(marked_opp_id != -1)
    {
        Point opp_pos = gameModel->findOpTeam(marked_opp_id)->getPosition();
        target = opp_pos + Measurements::unitVector(bp-opp_pos) * (2*ROBOT_RADIUS+50);
    }

    setMovementTargets(target, ang2ball);
    GenericMovementBehavior::perform(robot);
}

void MarkBot::updateMark()
{
    // For every opponent
    for(Robot* opp: gameModel->getOppTeam())
    {
        bool our_side = opp->getPosition().x < 0;
        bool has_ball = gameModel->getHasBall() != nullptr &&
                gameModel->getHasBall()->getID() == opp->getID();

        // Check if it is on our side
        if(our_side && !has_ball)
        {
            // Mark it if it's not already marked
            // and we are not already marking another opp
            if(mark_status[opp->getID()] == false && marked_opp_id == -1)
            {
                mark_status[opp->getID()] = true;
                marked_opp_id = opp->getID();
                break;
            }
        }
        // Unmark it if it's not on our side if it received the ball
        else
        {
            mark_status[opp->getID()] = false;

            if(marked_opp_id == opp->getID())
                marked_opp_id = -1;
        }
    }
}

bool MarkBot::isFinished()
{
    return false;
}
