#define _USE_MATH_DEFINES
#include <cmath>
#include "dribbletopoint.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/game_state.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"
namespace Skill {


DribbleToPoint::DribbleToPoint(Point& target, bool avoid_obstacles, bool prefer_forward_motion)
    : DribbleToPoint(&target, avoid_obstacles, prefer_forward_motion)
{
}

DribbleToPoint::DribbleToPoint(Point* target, bool avoid_obstacles, bool prefer_forward_motion)
    : target(target)
    , prefer_forward_motion(prefer_forward_motion)
    , state(move_to_ball)
{
    cmd.avoidObstacles = avoid_obstacles;
}

bool DribbleToPoint::perform(Robot* robot)
{
//    std::cout << "Dribbling" << std::endl;

    Point bp = Ball::getPosition();
    Point rp = robot->getPosition();
    float ang_to_ball = Measurements::angleBetween(rp,bp);
    float dist_to_ball = Measurements::distance(rp,bp);

    switch (state)
    {
    case move_to_ball:
    {
//        std::cout << "Dribble: Travel" << std::endl;

        robot->setDribble(false);

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 50;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 5*M_PI/180;

        if(dist_check && ang_check)
        {
            grasp_point =rp + (bp - rp) * 0.5;
            state = grasp;
            break;
        }
        else
        {
            cmd.setTarget(bp,ang_to_ball);
            cmd.avoidBall = false;
            robot->getPilot()->goToPose(cmd);
        }
        break;
    }
    case grasp:
    {
//        std::cout << "Dribble: Grasp" << std::endl;

        if(!targetIsAhead(ang_to_ball, rp)
        && safeToAdjust(bp, robot)
        && prefer_forward_motion)
            state = adjust1;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 75;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 20*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDribble(true);

        cmd.setTarget(grasp_point,ang_to_ball);
        cmd.avoidBall = false;
        cmd.velocity_multiplier = 0.3;
        robot->getPilot()->goToPose(cmd);
        if(robot->getPilot()->finisedLastCommand())
            state = move_to_target;

        break;
    }
    case move_to_target:
    {
//        std::cout << "Dribble: Move" << std::endl;

        if(!targetIsAhead(ang_to_ball, rp)
        && safeToAdjust(bp, robot)
        && prefer_forward_motion)
            state = adjust1;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 75;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 30*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDribble(true);

        float ang_to_target = Measurements::angleBetween(rp, *target);

        cmd.velocity_multiplier = 1;
        cmd.setTarget(*target, ang_to_target);
        cmd.avoidBall = false;
        robot->getPilot()->goToPose(cmd);
        break;
    }
    case adjust1:
    {
//        std::cout << "Dribble: Adjust1" << std::endl;

        float theta = Measurements::angleBetween(bp,rp);
        Point adjust_point = Point(bp.x + ROBOT_RADIUS*2 * cos(theta),
                                   bp.y + ROBOT_RADIUS*2 * sin(theta));

        robot->setDribble(false);
        cmd.velocity_multiplier = 1;
        cmd.setTarget(adjust_point, ang_to_ball);
        cmd.avoidBall = true;
        robot->getPilot()->goToPose(cmd);


        if(robot->getPilot()->finisedLastCommand())
            state = adjust2;

        break;
    }
    case adjust2:
    {
//        std::cout << "Dribble: Adjust2" << std::endl;

        float theta = Measurements::angleBetween(*target,bp);
        Point adjust_point = Point(bp.x + ROBOT_RADIUS*2 * cos(theta),
                                   bp.y + ROBOT_RADIUS*2 * sin(theta));

        robot->setDribble(false);
        cmd.velocity_multiplier = 1;
        cmd.setTarget(adjust_point, ang_to_ball);
        cmd.avoidBall = true;
        robot->getPilot()->goToPose(cmd);


        if(robot->getPilot()->finisedLastCommand()
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

bool DribbleToPoint::safeToAdjust(const Point& bp, Robot* robot)
{
    for(Robot* r : Robot::getAllRobots()){
        if(r->getTeamId()==robot->getTeamId()){
            if(r!=robot && Measurements::distance(bp, r->getPosition()) < ROBOT_RADIUS + Field::BALL_RADIUS + 50)
                return false;
        } else {
            if(Measurements::distance(bp, r->getPosition()) < 500)
                return false;
        }
    }

    return true;
}

}