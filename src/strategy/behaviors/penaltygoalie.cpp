#include "penaltygoalie.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/team.h"
#include "robot/robot.h"
#include "model/game_state.h"

PenaltyGoalie::PenaltyGoalie(Robot* robot) : Behavior(robot), GenericMovementBehavior(robot)
{
}

bool PenaltyGoalie::perform()
{
    robot->setDribble(false);
    Point bp = *game_state->getBall();
    float angleToBall = Measurements::angleBetween(robot, bp);

    Robot* kicker = game_state->getRobotWithBall();

    // Determine which opponent is taking the kick
    if(!kicker)
    {
        for(Robot* opp : robot->getOpponentRobots())
            if(Measurements::distance(kicker, bp) > Measurements::distance(opp, bp))
                kicker = opp;
    }

    // Define line segment along which goalie is allowed to move
    int goalie_x = -(Field::HALF_FIELD_LENGTH - ROBOT_RADIUS);
    Point p1 = Point(goalie_x, -Field::GOAL_LENGTH/2 - 50);
    Point p2 = Point(goalie_x,  Field::GOAL_LENGTH/2 + 50);

    // Line Intercept of kicker->bp and p1->p2
    // https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection
    float x1 = p1.x, y1 = p1.y;
    float x2 = p2.x, y2 = p2.y;

    float x3 = bp.x, y3 = bp.y;
    float x4, y4;

    auto gp = Field::getGoalPosition(OUR_SIDE);

    if(kicker)
    {
        x4 = kicker->getPosition().x;
        y4 = kicker->getPosition().y;
    }
    else
    {
        x4 = gp.x + Field::DEF_AREA_WIDTH;
        y4 = 0;
    }

    // Px is the point at which the lines kicker->bp and p1->p2 intercept
    float Px_num = (x1*y2 - y1*x2)*(x3 - x4) - (x1 - x2)*(x3*y4 - y3*x4);
    float Py_num = (x1*y2 - y1*x2)*(y3 - y4) - (y1 - y2)*(x3*y4 - y3*x4);
    float dem = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);

    // Stay at the centre of the goalpost if the opponent is not aiming
    // somewhere inside the goal-post.
    Point block_point = gp + Point(ROBOT_RADIUS, 0);

    if(dem != 0)    // Lines are not parallel
    {
        // Check if intersection is within defensible stretch
        float Py = Py_num/dem;
        float Px = Px_num/dem;

        if(Py < Field::GOAL_LENGTH/2 && Py > -Field::GOAL_LENGTH/2)
            block_point = Point(Px, Py);
    }

//    GuiInterface::getGuiInterface()->drawLine(block_point, bp);

    cmd.velocity_multiplier = 1.5;
    cmd.setTarget(block_point, angleToBall);
    cmd.avoidBall = cmd.avoidObstacles = false;
    GenericMovementBehavior::perform();
    return isFinished();
}

bool PenaltyGoalie::isFinished()
{
    DefenceArea our_da(TEAM_DEFFENCE_AREA);
    auto ball = game_state->getBall();
    if(ball->getSpeed() < 100 &&
       our_da.contains(*ball, -ROBOT_RADIUS))
        return true;
    return false;
}

string PenaltyGoalie::getName() {
    return "Penalty Goalie";
}
