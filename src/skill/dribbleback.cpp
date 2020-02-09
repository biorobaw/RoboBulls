#include "dribbleback.h"
#include "src/model/ball.h"
#include "src/model/field.h"

namespace Skill
{

DribbleBack::DribbleBack(Point& target)
    : DribbleBack(&target)
{
}

DribbleBack::DribbleBack(Point* target)
    : target(target)
{
}

bool DribbleBack::perform(Robot* robot)
{
//    std::cout << "Dribble Back" << std::endl;

    Point bp = Ball::getPosition();
    Point rp = robot->getPosition();
    float ang_to_ball = Measurements::angleBetween(rp,bp);
    float dist_to_ball = Measurements::distance(rp,bp);

    switch (state)
    {
    case move_to_ball:
    {
//        std::cout << "Dribble Back: Move to Ball" << std::endl;

        robot->setDribble(false);

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 50;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 5*M_PI/180;

        if(dist_check && ang_check)
        {
            grasp_point = rp + (bp-rp) * 0.7;
            state = grasp;
            break;
        }
        else
        {
            Point move_point = bp + (*target - bp)*(ROBOT_RADIUS/Measurements::mag(*target - bp));
            move_skill.setVelocityMultiplier(1.0);
            move_skill.updateGoal(move_point, ang_to_ball, false, false);
            move_skill.perform(robot);
        }
        break;
    }
    case grasp:
    {
//        std::cout << "Dribble Back: Grasp" << std::endl;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 75;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 20*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            break;
        }

        robot->setDribble(true);

        move_skill.updateGoal(grasp_point, ang_to_ball, false, false);
        move_skill.setVelocityMultiplier(0.2);

        if(move_skill.perform(robot))
            state = move_back;

        break;
    }
    case move_back:
    {
//        std::cout << "Dribble Back: Move" << std::endl;

        bool dist_check = dist_to_ball < ROBOT_RADIUS + Field::BALL_RADIUS + 75;
        bool ang_check = Measurements::angleDiff(ang_to_ball, robot->getOrientation()) < 30*M_PI/180;

        if(!dist_check || !ang_check)
        {
            state = move_to_ball;
            prev_vel = 0;
            break;
        }

        robot->setDribble(true);

        float vel = fmax(-3, prev_vel - 0.05);
        prev_vel = vel;

        robot->setLF(vel);
        robot->setLB(vel);
        robot->setRF(vel);
        robot->setRB(vel);
    }

    }

    if(Measurements::distance(bp, *target) < 300)
        return true;
    return false;
}

}
