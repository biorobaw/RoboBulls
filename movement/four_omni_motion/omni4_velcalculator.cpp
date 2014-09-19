#include "movement/four_omni_motion/omni4_velcalculator.h"

namespace Movement
{

FourWheelCalculator::FourWheelCalculator()
{
    this->angle_error_deque.push_back(0);
    this->dist_error_deque.push_back(0);
}

fourWheelVels FourWheelCalculator::calculateVels
    (Robot* rob, Point goalPoint, float theta_goal, Type moveType)
{
	return calculateVels(rob, goalPoint.x, goalPoint.y, theta_goal, moveType);
}

fourWheelVels FourWheelCalculator::calculateVels
    (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType)
{
    switch (moveType)
    {
        case Type::facePoint:
            return facePointCalc(rob,x_goal,y_goal,theta_goal);
            break;
        default:
            return defaultCalc(rob,x_goal,y_goal,theta_goal);
    }
}


fourWheelVels FourWheelCalculator::defaultCalc
    (Robot* rob, float x_goal, float y_goal, float theta_goal)
{
    //Current Position
    double x_current = rob->getRobotPosition().x;
    double y_current = rob->getRobotPosition().y;
    double theta_current = rob->getOrientation();

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_to_goal = Measurments::distance(rp,gp);
    angle_to_goal = Measurments::angleBetween(rp,gp);

    //PID
    //calc_error();

    //Inertial Frame Velocities
    double x_vel = (distance_to_goal+dist_error_integral)*cos(angle_to_goal);
    double y_vel = (distance_to_goal+dist_error_integral)*sin(angle_to_goal);
    double theta_vel = theta_goal-theta_current;
    if (abs(Measurments::angleDiff(theta_goal,theta_current))<abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 300)
    {
        x_vel *= 0.5;
        y_vel *= 0.5;
    }

    //cout << dist_error_integral << endl;

    //Set tolerances
    //if (Measurments::isClose(rp,gp,10)) x_vel = y_vel = 0;
    //if (abs(Measurments::angleDiff(theta_goal,theta_current))<ROT_TOLERANCE) theta_vel = 0;

    // Robot Frame Velocities
    double x_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double y_vel_robot = -sin(theta_current)*x_vel+cos(theta_current)*y_vel;

    //Wheel Velocity Calculations
    double RF = (sin(frnt_axl_offset) * y_vel_robot + cos(frnt_axl_offset)*x_vel_robot + wheel_radius*theta_vel);
    double LF = -(sin(frnt_axl_offset) * y_vel_robot - cos(frnt_axl_offset)*x_vel_robot + wheel_radius*theta_vel);
    double LB = -(-sin(rear_axl_offset) * y_vel_robot - cos(rear_axl_offset)*x_vel_robot + wheel_radius*theta_vel);
    double RB = (-sin(rear_axl_offset) * y_vel_robot + cos(rear_axl_offset)*x_vel_robot + wheel_radius*theta_vel);

    //Normalize wheel velocities
    unsigned int max_mtr_spd = 100;
    if (abs(LF)>max_mtr_spd)
    {
        LB=(max_mtr_spd/abs(LF))*LB;
        RF=(max_mtr_spd/abs(LF))*RF;
        RB=(max_mtr_spd/abs(LF))*RB;
        LF=(max_mtr_spd/abs(LF))*LF;
    }
    if (abs(LB)>max_mtr_spd)
    {
        LF=(max_mtr_spd/abs(LB))*LF;
        RF=(max_mtr_spd/abs(LB))*RF;
        RB=(max_mtr_spd/abs(LB))*RB;
        LB=(max_mtr_spd/abs(LB))*LB;
    }
    if (abs(RF)>max_mtr_spd)
    {
        LF=(max_mtr_spd/abs(RF))*LF;
        LB=(max_mtr_spd/abs(RF))*LB;
        RB=(max_mtr_spd/abs(RF))*RB;
        RF=(max_mtr_spd/abs(RF))*RF;
    }
    if (abs(RB)>max_mtr_spd)
    {
        LF=(max_mtr_spd/abs(RB))*LF;
        LB=(max_mtr_spd/abs(RB))*LB;
        RF=(max_mtr_spd/abs(RB))*RF;
        RB=(max_mtr_spd/abs(RB))*RB;
    }

    //Create and return result container
    fourWheelVels vels;
    vels.LB = LB;
    vels.LF = LF;
    vels.RB = RB;
    vels.RF = RF;
    return vels;
}

fourWheelVels FourWheelCalculator::facePointCalc
    (Robot* rob, float x_goal, float y_goal, float theta_goal)
{
    //Current Position
    double x_current = rob->getRobotPosition().x;
    double y_current = rob->getRobotPosition().y;
    double theta_current = rob->getOrientation();

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_to_goal = Measurments::distance(rp,gp);
    angle_to_goal = Measurments::angleBetween(rp,gp);

    //PID
    //calc_error();

    //Interial Frame Velocities
    double x_vel = (distance_to_goal+dist_error_integral)*cos(angle_to_goal);
    double y_vel = (distance_to_goal+dist_error_integral)*sin(angle_to_goal);
    double theta_vel = 1.5*(theta_goal-theta_current);
    if (abs(Measurments::angleDiff(theta_goal,theta_current))<abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 300)
    {
        x_vel *= 0.5;
        y_vel *= 0.5;
    }

    // Rotate first
    if (abs(Measurments::angleDiff(theta_goal,theta_current))>0.15)
    {
        x_vel *= 0.2;
        y_vel *= 0.2;
    }

    //cout << dist_error_integral << endl;

    //Set tolerances
    //if (Measurments::isClose(rp,gp,10)) x_vel = y_vel = 0;
    //if (abs(Measurments::angleDiff(theta_goal,theta_current))<ROT_TOLERANCE) theta_vel = 0;

    // Robot Frame Velocities
    double x_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double y_vel_robot = -sin(theta_current)*x_vel+cos(theta_current)*y_vel;

    //Wheel Velocity Calculations
    double RF = (sin(frnt_axl_offset) * y_vel_robot + cos(frnt_axl_offset)*x_vel_robot + wheel_radius*theta_vel);
    double LF = -(sin(frnt_axl_offset) * y_vel_robot - cos(frnt_axl_offset)*x_vel_robot + wheel_radius*theta_vel);
    double LB = -(-sin(rear_axl_offset) * y_vel_robot - cos(rear_axl_offset)*x_vel_robot + wheel_radius*theta_vel);
    double RB = (-sin(rear_axl_offset) * y_vel_robot + cos(rear_axl_offset)*x_vel_robot + wheel_radius*theta_vel);

    //Normalize wheel velocities
    unsigned int max_mtr_spd = 100;
    if (abs(LF)>max_mtr_spd)
    {
        LB=(max_mtr_spd/abs(LF))*LB;
        RF=(max_mtr_spd/abs(LF))*RF;
        RB=(max_mtr_spd/abs(LF))*RB;
        LF=(max_mtr_spd/abs(LF))*LF;
    }
    if (abs(LB)>max_mtr_spd)
    {
        LF=(max_mtr_spd/abs(LB))*LF;
        RF=(max_mtr_spd/abs(LB))*RF;
        RB=(max_mtr_spd/abs(LB))*RB;
        LB=(max_mtr_spd/abs(LB))*LB;
    }
    if (abs(RF)>max_mtr_spd)
    {
        LF=(max_mtr_spd/abs(RF))*LF;
        LB=(max_mtr_spd/abs(RF))*LB;
        RB=(max_mtr_spd/abs(RF))*RB;
        RF=(max_mtr_spd/abs(RF))*RF;
    }
    if (abs(RB)>max_mtr_spd)
    {
        LF=(max_mtr_spd/abs(RB))*LF;
        LB=(max_mtr_spd/abs(RB))*LB;
        RF=(max_mtr_spd/abs(RB))*RF;
        RB=(max_mtr_spd/abs(RB))*RB;
    }

    //Create and return result container
    fourWheelVels vels;
    vels.LB = LB;
    vels.LF = LF;
    vels.RB = RB;
    vels.RF = RF;
    return vels;
}

void FourWheelCalculator::calc_error()
{
    //Integral Error for distance
    if (dist_error_deque.size() <= dist_error_maxsize)
    {
        dist_error_integral += distance_to_goal;
        dist_error_deque.push_back(distance_to_goal);
    }
    else
    {
        dist_error_integral -= dist_error_deque.front();
        dist_error_integral += distance_to_goal;
        dist_error_deque.pop_front();
        dist_error_deque.push_back(distance_to_goal);
    }

    //Integral Error for orientation
    if (angle_error_deque.size() <= angle_error_maxsize)
    {
        angle_error_integral += angle_to_goal;
        angle_error_deque.push_back(angle_to_goal);
    }
    else
    {
        angle_error_integral -= angle_error_deque.front();
        angle_error_integral += angle_to_goal;
        angle_error_deque.pop_front();
        angle_error_deque.push_back(distance_to_goal);
    }
}

}
