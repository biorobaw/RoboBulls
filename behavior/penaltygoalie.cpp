#include "penaltygoalie.h"
#include "model/ball.h"

PenaltyGoalie::PenaltyGoalie()
{
}

void PenaltyGoalie::perform(Robot* robot)
{
    robot->setDribble(false);
    Point bp = Ball::getPosition();
    float angleToBall = Measurements::angleBetween(robot, bp);

    Robot* kicker = Ball::getRobotWithBall();

    // Determine which opponent is taking the kick
    if(!kicker)
    {
        for(Robot* opp : robot->getOpponentTeam()->getRobots())
            if(Measurements::distance(kicker, bp) > Measurements::distance(opp, bp))
                kicker = opp;
    }

    // Define line segment along which goalie is allowed to move
    Point p1 = Point(-HALF_FIELD_LENGTH + ROBOT_RADIUS, -GOAL_WIDTH/2 - 50);
    Point p2 = Point(-HALF_FIELD_LENGTH + ROBOT_RADIUS,  GOAL_WIDTH/2 + 50);

    // Line Intercept of kicker->bp and p1->p2
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;

    float x3 = bp.x, y3 = bp.y;
    float x4, y4;
    if(kicker)
    {
        x4 = kicker->getPosition().x;
        y4 = kicker->getPosition().y;
    }
    else
    {
        x4 = gameModel->getMyGoal().x + DEF_AREA_RADIUS;
        y4 = 0;
    }

    // Px is the point at which the lines kicker->bp and p1->p2 intercept
    float Px_num = (x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4);
    float Py_num = (x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4);
    float dem = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    // Stay at the centre of the goalpost if the opponent is not aiming
    // somewhere inside the goal-post.
    Point block_point = gameModel->getMyGoal() + Point(ROBOT_RADIUS, 0);

    if(dem != 0)    // Lines are not parallel
    {
        // Check if intersection is within defensible stretch
        float Py = Py_num/dem;
        float Px = Px_num/dem;

        if(Py < GOAL_WIDTH/2 && Py > -GOAL_WIDTH/2)
            block_point = Point(Px, Py);
    }

    GuiInterface::getGuiInterface()->drawLine(block_point, bp);

    setVelocityMultiplier(1.5);
    setMovementTargets(block_point, angleToBall, false, false);
    GenericMovementBehavior::perform(robot);
}

bool PenaltyGoalie::isFinished()
{
    DefenceArea our_da(TEAM_DEFFENCE_AREA);
    if(Ball::getSpeed() < 100 && our_da.contains(Ball::getPosition(), -ROBOT_RADIUS))
        return true;
    return false;
}
