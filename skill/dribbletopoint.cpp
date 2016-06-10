#include "dribbletopoint.h"

namespace Skill {


DribbleToPoint::DribbleToPoint(Point& target)
    : DribbleToPoint(&target)
{
}

DribbleToPoint::DribbleToPoint(Point* target)
    : target(target)
    , state(move_to_ball)
{

}

bool DribbleToPoint::perform(Robot* robot)
{
//    std::cout << "Dribbling" << std::endl;

    Point bp = gameModel->getBallPoint();
    Point rp = robot->getPosition();
    float ang_to_ball = Measurements::angleBetween(rp,bp);
    float dist_to_ball = Measurements::distance(rp,bp);

    switch (state)
    {
    case move_to_ball:
    {
//        std::cout << "Dribble: Travel" << std::endl;

        robot->setDrible(false);

        bool dist_check = dist_to_ball < ROBOT_RADIUS + BALL_RADIUS + 50;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 5*M_PI/180;

        if(dist_check && ang_check)
            state = grasp;
        else
        {
            move_skill.recreate(bp, ang_to_ball, true, false);
            move_skill.perform(robot);
            grasp_point = bp;
        }
        break;
    }
    case grasp:
    {
//        std::cout << "Dribble: Grasp" << std::endl;

        if(!targetIsAhead(ang_to_ball, rp)
        && safeToAdjust(bp, robot->getID()))
            state = adjust1;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + BALL_RADIUS + 75;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 20*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDrible(true);

        move_skill.recreate(grasp_point, ang_to_ball, true, false);
        move_skill.setVelocityMultiplier(0.3);

        if(move_skill.perform(robot))
            state = move_to_target;

        break;
    }
    case move_to_target:
    {
//        std::cout << "Dribble: Move" << std::endl;

        if(!targetIsAhead(ang_to_ball, rp)
        && safeToAdjust(bp, robot->getID()))
            state = adjust1;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + BALL_RADIUS + 75;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 30*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDrible(true);

        float ang_to_target = Measurements::angleBetween(rp, *target);

        move_skill.recreate(*target, ang_to_target, true, false);
        move_skill.setMovementTolerances(DIST_TOLERANCE, 1*M_PI/180);
        move_skill.perform(robot, Movement::Type::dribble);
        break;
    }
    case adjust1:
    {
//        std::cout << "Dribble: Adjust1" << std::endl;
        float theta = Measurements::angleBetween(bp,rp);
        Point adjust_point = Point(bp.x + ROBOT_RADIUS*2 * cos(theta),
                                   bp.y + ROBOT_RADIUS*2 * sin(theta));

        robot->setDrible(false);
        move_skill.recreate(adjust_point, ang_to_ball, true, true);

        if(move_skill.perform(robot))
            state = adjust2;

        break;
    }
    case adjust2:
    {
//        std::cout << "Dribble: Adjust2" << std::endl;
        float theta = Measurements::angleBetween(*target,bp);
        Point adjust_point = Point(bp.x + ROBOT_RADIUS*2 * cos(theta),
                                   bp.y + ROBOT_RADIUS*2 * sin(theta));

        robot->setDrible(true);
        move_skill.recreate(adjust_point, ang_to_ball, true, true);

        if(move_skill.perform(robot)
        || Measurements::distance(rp, bp) > ROBOT_RADIUS*3)
            state = move_to_ball;

        break;
    }
    }

    if(Measurements::distance(bp, *target) < 300)
        return true;
    return false;
}

bool DribbleToPoint::targetIsAhead(const float& ang_to_ball, const Point& rp)
{
    float ang_to_target = Measurements::angleBetween(rp, *target);
    return fabs(Measurements::angleDiff(ang_to_ball,ang_to_target)) < M_PI_4;
}

bool DribbleToPoint::safeToAdjust(const Point& bp, const int rob_id)
{
    for(Robot* opp_rob : gameModel->getOppTeam())
    {
        if(Measurements::distance(bp, opp_rob->getPosition()) < 500)
            return false;
    }

    for(Robot* teammate : gameModel->getMyTeam())
    {
        if(teammate->getID() != rob_id)
            if(Measurements::distance(bp, teammate->getPosition()) < ROBOT_RADIUS + BALL_RADIUS + 50)
                return false;
    }

    return true;
}
}


