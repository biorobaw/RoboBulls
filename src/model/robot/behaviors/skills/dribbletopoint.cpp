#define _USE_MATH_DEFINES
#include <cmath>
#include "dribbletopoint.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/game_state.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/team/team.h"
#include "model/game_state.h"
#include "utilities/measurements.h"



DribbleToPoint::DribbleToPoint(Robot* robot, Point& target, bool avoid_obstacles, bool prefer_forward_motion)
    : DribbleToPoint(robot, &target, avoid_obstacles, prefer_forward_motion)
{
}

DribbleToPoint::DribbleToPoint(Robot* robot, Point* target, bool avoid_obstacles, bool prefer_forward_motion)
    : Behavior(robot)
    , target(target)
    , prefer_forward_motion(prefer_forward_motion)
    , state(move_to_ball)
{
    cmd.avoid_obstacles = avoid_obstacles;
}

bool DribbleToPoint::perform()
{
//    std::cout << "Dribbling" << std::endl;

    Point bp = *ball;
    Point rp = *robot;
    float ang_to_ball = Measurements::angleBetween(rp,bp);
    float dist_to_ball = Measurements::distance(rp,bp);

    switch (state)
    {
    case move_to_ball:
    {
//        std::cout << "Dribble: Travel" << std::endl;

        robot->setDribble(false);

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 50;
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < 5*M_PI/180;

        if(dist_check && ang_check)
        {
            grasp_point =rp + (bp - rp) * 0.5;
            state = grasp;
            break;
        }
        else
        {
            cmd.setTarget(bp,ang_to_ball);
            cmd.avoid_ball = false;
            robot->goToPose(cmd);
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
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 20*M_PI/180);

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDribble(true);

        cmd.setTarget(grasp_point,ang_to_ball);
        cmd.avoid_ball = false;
        cmd.velocity_multiplier = 0.3;
        robot->goToPose(cmd);
        if(robot->completedGoToPoseCmd())
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
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 30*M_PI/180);

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDribble(true);

        float ang_to_target = Measurements::angleBetween(rp, *target);

        cmd.velocity_multiplier = 1;
        cmd.setTarget(*target, ang_to_target);
        cmd.avoid_ball = false;
        robot->goToPose(cmd);
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
        cmd.avoid_ball = true;
        robot->goToPose(cmd);


        if(robot->completedGoToPoseCmd())
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
        cmd.avoid_ball = true;
        robot->goToPose(cmd);


        if(robot->completedGoToPoseCmd()
        || Measurements::distance(rp, bp) > ROBOT_RADIUS*3)
            state = move_to_ball;

        break;
    }
    }

    return isFinished();
}

bool DribbleToPoint::targetIsAhead(const float& ang_to_ball, const Point& rp)
{
    float ang_to_target = Measurements::angleBetween(rp, *target);
    return fabs(Measurements::angleDiff(ang_to_ball,ang_to_target)) < M_PI_4;
}

bool DribbleToPoint::safeToAdjust(const Point& bp, Robot* robot)
{
    for(Robot* r : game_state->getFieldRobots()){
        if(r->getTeamId()==robot->getTeamId()){
            if(r!=robot && Measurements::distance(bp, *r) < ROBOT_RADIUS + Field::BALL_RADIUS + 50)
                return false;
        } else {
            if(Measurements::distance(bp, *r) < 500)
                return false;
        }
    }

    return true;
}


bool DribbleToPoint::isFinished(){
    return Measurements::distance(*ball, *target) < 300;
}
string DribbleToPoint::getName(){
    return "Dribble to point";
}


