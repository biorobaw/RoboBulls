#define _USE_MATH_DEFINES
#include <cmath>
#include "dribbleback.h"
#include "model/ball.h"
#include "model/field.h"
#include "model/game_state.h"
#include "model/robot/robot.h"
#include "model/robot/navigation/robot_pilot.h"
#include "model/team/team.h"
#include "model/game_state.h"
#include "utilities/measurements.h"



DribbleBack::DribbleBack(Robot* robot, Point& target)
    : DribbleBack(robot, &target)
{
}

DribbleBack::DribbleBack(Robot* robot, Point* target)
    : Behavior(robot), target(target)
{
}

bool DribbleBack::perform()
{
//    std::cout << "Dribble Back" << std::endl;

    Point bp = *ball;
    Point rp = *robot;
    float ang_to_ball = Measurements::angleBetween(rp,bp);
    float dist_to_ball = Measurements::distance(rp,bp);

    switch (state)
    {
    case move_to_ball:
    {
//        std::cout << "Dribble Back: Move to Ball" << std::endl;

        robot->setDribble(false);

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 50;
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < 5*M_PI/180;

        if(dist_check && ang_check)
        {
            grasp_point = rp + (bp-rp) * 0.7;
            state = grasp;
            break;
        }
        else
        {
            Point move_point = bp + (*target - bp)*(ROBOT_RADIUS/Measurements::mag(*target - bp));
            cmd.velocity_multiplier = 1;
            cmd.setTarget(move_point,ang_to_ball);
            cmd.avoid_ball = cmd.avoid_obstacles = false;
            robot->goToPose(cmd);
        }
        break;
    }
    case grasp:
    {
//        std::cout << "Dribble Back: Grasp" << std::endl;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 75;
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < 20*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDribble(true);

        cmd.setTarget(grasp_point,ang_to_ball);
        cmd.avoid_ball = cmd.avoid_obstacles = false;
        cmd.velocity_multiplier = 0.2;
        robot->goToPose(cmd);

        if(robot->completedGoToPoseCmd())
            state = move_back;

        break;
    }
    case move_back:
    {
//        std::cout << "Dribble Back: Move" << std::endl;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 75;
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < 30*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            prev_vel = 0;
            break;
        }

        robot->setDribble(true);

        float vel = fmax(-3, prev_vel - 0.05);
        prev_vel = vel;

        robot->setTargetVelocityGlobal(Point(0,0),vel/4);

    }

    }


    return isFinished();
}

bool DribbleBack::isFinished(){
    return Measurements::distance(*ball, *target) < 300;
}
string DribbleBack::getName(){
    return "Dribble back";
}



