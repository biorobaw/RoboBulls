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
#include "iostream"



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
    //std::cout << "Dribble Back" << std::endl;

    Point bp = *ball;
    Point rp = *robot;
    float ang_to_ball = Measurements::angleBetween(rp,bp);
    float dist_to_ball = Measurements::distance(rp,bp);

    switch (state)
    {
    case move_to_ball:
    {
        //std::cout << "Dribble Back: Move to Ball" << std::endl;
        if(!robot->hasBall())
            robot->setDribble(false);

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + DIST_TOLERANCE*2; // was 50
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < ROT_TOLERANCE;
        //std::cout << "dist to ball: " << dist_to_ball << " tolerance total: " << ROBOT_RADIUS + Field::BALL_RADIUS + DIST_TOLERANCE*2 << std::endl;

        if(dist_check && ang_check)
        {
            grasp_point = rp + (bp-rp) * 0.7;
            state = grasp;
            break;
        }
        else
        {
            Point move_point = bp + (*target - bp)*(ROBOT_RADIUS/Measurements::mag(*target - bp));
            //std::cout << "move point: " << move_point.x << ", "<< move_point.y << " robot: " << rp.x << ", "<< rp.y << std::endl;
            cmd.velocity_multiplier = 1;
            cmd.setTarget(move_point,ang_to_ball);
            cmd.avoid_ball = cmd.avoid_obstacles = false;
            robot->goToPose(cmd);
        }
        break;
    }
    case grasp:
    {
        //std::cout << "Dribble Back: Grasp" << std::endl;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + DIST_TOLERANCE*2; // was 75
        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < ROT_TOLERANCE; //was 20*m_pi/180

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
        //std::cout << "Dribble Back: Move" << std::endl;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + DIST_TOLERANCE*2; //was 75


        bool ang_check = fabs(Measurements::angleDiff(ang_to_ball, robot->getOrientation())) < ROT_TOLERANCE; //was 30*m_pi/180

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            prev_vel = 0;
            break;
        }

        robot->setDribble(true);

        /*float vel = fmax(-3, prev_vel - 0.05);
        prev_vel = vel;

        robot->setTargetVelocityGlobal(Point(0,0),vel/4);*/
        cmd.setTarget(*target,ang_to_ball);
        cmd.avoid_ball = cmd.avoid_obstacles = false;
        cmd.velocity_multiplier = -0.2;
        robot->goToPose(cmd);

    }

    }


    return isFinished();
}

bool DribbleBack::isFinished(){
    if (Measurements::distance(*ball, *target) < 300) std::cout << "finished"<<std::endl;
    return Measurements::distance(*ball, *target) < 300;
}
string DribbleBack::getName(){
    return "Dribble back";
}



