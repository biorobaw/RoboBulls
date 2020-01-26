#include "wall.h"
#include "model/ball.h"

Wall::WallPoint Wall::wall_array[2];
bool Wall::being_cleared = false;

Wall::Wall()
{
    kick_skill = new Skill::KickToPointOmni(&kickPoint);
}

Wall::~Wall()
{
    for(WallPoint& wp: wall_array)
        if(wp.id == id)
            wp.id = -1;

    delete kick_skill;
}

void Wall::perform(Robot * robot)
{
    id = robot->getID();
    Point bp = Ball::getPosition();
    float ang2ball = Measurements::angleBetween(robot, bp);
    switch(state)
    {
    case wall:
    {
        // Evaluate transition to clear
        if(shouldClear(robot) && !being_cleared)
        {
            being_cleared = true;
            state = clear;
            break;
        }

        calcWallPoints(robot);

        for(WallPoint& wp: wall_array)
        {
            // If wall point is within field
            if(Comparisons::isPointInsideField(wp.position))
            {
                // If it is not already being occupied by a teammate
                // and we are not already occupying a wall point
                if(wp.id == -1 && !occupying)
                {
                    wp.id = robot->getID();
                    occupying = true;
                }

                // If we are occupying it, move to it
                if(wp.id == robot->getID())
                    setMovementTargets(wp.position, ang2ball);
            }
        }

        if(occupying)
            GenericMovementBehavior::perform(robot);
    }
    break;

    case clear:
    {
        // Find reachable teammates
        std::vector<Robot*> candidates;

        std::vector<Robot*> obstacles;
        for(Robot* r : Robot::getAllRobots()) obstacles.push_back(r);

        for(Robot* tmate: robot->getTeam()->getRobots())
        {
            if (tmate->getID() != robot->getID())
            {
                auto remove_it = std::remove_if(obstacles.begin(), obstacles.end(), [&](Robot* r)
                {
                    return (r->getTeam()==robot->getTeam()) && (r->getID() == tmate->getID() || r->getID() == robot->getID());
                });

                obstacles.erase(remove_it, obstacles.end());

                Robot* obst = Measurements::robotInPath(obstacles, bp, tmate->getPosition(), ROBOT_RADIUS + BALL_RADIUS + 20);

                obstacles.push_back(tmate);
                obstacles.push_back(robot);

                if (obst == nullptr)
                    candidates.push_back(tmate);
            }
        }

        // Find farthest robot from among the reachable teammates
        Robot* farthest = nullptr;
        if(!candidates.empty())
        {
            farthest = candidates.front();
            for(Robot* r: candidates)
            {
                if(r->getPosition().x > farthest->getPosition().x)
                    farthest = r;
            }
        }

        if(farthest != nullptr && farthest->getPosition().x - robot->getPosition().x > 500)
            kickPoint = farthest->getPosition();
        else
            kickPoint = Point(3000, 0);

        // Peform kick skill and go back to walling if
        // we are done or the ball no longer needs clearing
        if(kick_skill->perform(robot) || shouldStopClearing(robot))
        {
            being_cleared = false;
            state = wall;
        }
    }
    break;
    }
}

void Wall::calcWallPoints(Robot* robot)
{
    Point bp = Ball::getPosition();
    Robot* goalie = robot->getTeam()->getRobotByRole(RobotRole::GOALIE);
    Point gp = goalie ? goalie->getPosition() : gameModel->getMyGoal();


    DefenceArea da(TEAM_DEFFENCE_AREA);

    // Get interception of line bp->goalie
    // with defence area edge
    std::vector<Point> intercepts = da.lineSegmentIntercepts(bp, gp);
    Point intercept;

    if(intercepts.empty())
    {
        // The goalie would have to be quite out of place
        // for no intercepts, so position between the ball
        // and the center of the goal
        std::vector<Point> temp = da.lineIntercepts(bp, gameModel->getMyGoal());
        if(temp.empty())
            intercept = Point (DEF_AREA_RADIUS + 3*ROBOT_RADIUS, 0);
        else
            intercept = temp.front();
    }
    else if (intercepts.size() == 1)
    {
        // Accept the singular interception
        intercept = intercepts.front();
    }
    else
    {
        // If we get two intercepts, choose the one closer to the ball
        if(Measurements::distance(bp,intercepts.front())
        < Measurements::distance(bp,intercepts.back()) )
            intercept = intercepts.front();
        else
            intercept = intercepts.back();
    }

    // Wall Geometry Variables
    float spacing = 2*ROBOT_RADIUS;
    float dist_to_def_area = 2*ROBOT_RADIUS + 50;

    // Space out farther if the goal-keeper must take a shot
    if(spaceForGoalKick())
        spacing *= 2;

    // Vector parallel to bp->gp
    Point parallel = Measurements::unitVector(bp-gp) * dist_to_def_area;

    // Vector perpendicular to bp->gp
    Point unit_orth = Measurements::unitVector(Point(-(bp-gp).y, (bp-gp).x));

    // Update wall
    wall_array[0].position = intercept + parallel + unit_orth * spacing;
    wall_array[1].position = intercept + parallel - unit_orth * spacing;
}

bool Wall::spaceForGoalKick()
{
    // Determine if the wall should space out for a goal-kick
    Robot* possessor = Ball::getRobotWithBall();
    DefenceArea da(TEAM_DEFFENCE_AREA);

    // Position for a goal-kick if the goal-keeper has the ball
    // and the ball is is inside the defence area
    bool b1 = possessor != nullptr && possessor->isGoalie();
    bool b2 = da.contains(Ball::getPosition());

    return b1 && b2;
}

bool Wall::shouldClear(Robot* robot)
{
    // Clear the ball if it is stopped on our side of the field
    // and we are the nearest robot to it and the ball is
    // free or if we possess it.
    Point bp = Ball::getPosition();
    DefenceArea da(TEAM_DEFFENCE_AREA);
    Robot* possessor = Ball::getRobotWithBall();

    bool b1 = Ball::getSpeed() < 10;
    bool b2 = bp.x < -1000 && !da.contains(bp, 2*ROBOT_RADIUS);
    bool b3 = Comparisons::distanceBall().minInTeam(robot->getTeam())->getID() == robot->getID();
    bool b4 = possessor == nullptr || (possessor != nullptr  &&
                possessor->getID() == robot->getID());

    return b1 && b2 && b3 && b4;
}

bool Wall::shouldStopClearing(Robot* robot)
{
    // We go back to other tasks if the ball is no longer
    // on our side of the field or if an opponent
    // has the ball or is too close the defence area
    DefenceArea da(TEAM_DEFFENCE_AREA);
    Robot* possessor = Ball::getRobotWithBall();

    bool b1 = Ball::getPosition().x > -1000;
    bool b2 = Ball::getRobotWithBall() != nullptr
            && Ball::getRobotWithBall()->getTeam()!=robot->getTeam();
    bool b3 = da.contains(Ball::getPosition(), ROBOT_RADIUS * 2);
    bool b4 = possessor != nullptr && possessor->getID() != robot->getID();

    return b1 || b2 || b3 || b4;
}

bool Wall::isFinished()
{
    return being_cleared;
}
