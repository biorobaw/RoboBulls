#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "utilities/debug.h"
#include "include/config/tolerances.h"
using std::abs;

namespace Movement
{

#define FOUR_WHEEL_DEBUG 0

//Multiplier for theta_vel in defaultCalc (set 10x actual)
int THETA_MULT = 3;

//Multiplier for theta_vel in facePointCalc (set 10x actual)
int THETA_MULT2 = 10;

//Multiplier for x_vel and y_vel in defaultCalc (set 10x actual)
int XY_MULT = 3;

FourWheelCalculator::FourWheelCalculator()
{
    //These seem okay; taking out of registration for now
    //debug::registerVariable("fwc_xy", &XY_MULT);
    //debug::registerVariable("fwc_theta", &THETA_MULT);
    //debug::registerVariable("fwc_theta2", &THETA_MULT2);
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

    //Inertial Frame Velocities
    double x_vel = (distance_to_goal)*cos(angle_to_goal);
    double y_vel = (distance_to_goal)*sin(angle_to_goal);
    double theta_vel = Measurments::angleDiff(theta_current,theta_goal);
    if (abs(Measurments::angleDiff(theta_goal,theta_current))<abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 700)
    {
        x_vel *= ((float)XY_MULT / 10);
        y_vel *= ((float)XY_MULT / 10);
        theta_vel *= ((float)THETA_MULT / 10);
    }

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;

    //Wheel Velocity Calculations
    double RF =  (-sin(RF_offset) * x_vel_robot + cos(RF_offset)*y_vel_robot + wheel_radius*theta_vel);
    double LF = -(-sin(LF_offset) * x_vel_robot + cos(LF_offset)*y_vel_robot + wheel_radius*theta_vel);
    double LB = -(-sin(LB_offset) * x_vel_robot + cos(LB_offset)*y_vel_robot + wheel_radius*theta_vel);
    double RB =  (-sin(RB_offset) * x_vel_robot + cos(RB_offset)*y_vel_robot + wheel_radius*theta_vel);

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
    double x_vel = (distance_to_goal)*cos(angle_to_goal);
    double y_vel = (distance_to_goal)*sin(angle_to_goal);
    double theta_vel = Measurments::angleDiff(theta_current,theta_goal);
    if (abs(Measurments::angleDiff(theta_goal,theta_current))<abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 300)
    {
        x_vel *= 0.7;
        y_vel *= 0.7;

    }

    // Focus on rotation
    double vel = sqrt(x_vel*x_vel+y_vel*y_vel);
    if (abs(Measurments::angleDiff(theta_goal,theta_current))>ROT_TOLERANCE*0.5 && vel > 40)
    {
        x_vel = 90*cos(angle_to_goal);
        y_vel = 90*sin(angle_to_goal);
        theta_vel *= ((float)THETA_MULT2 / 10);
    }

    //cout << dist_error_integral << endl;

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;

    //Wheel Velocity Calculations
    double RF =  (-sin(RF_offset) * x_vel_robot + cos(RF_offset)*y_vel_robot + wheel_radius*theta_vel);
    double LF = -(-sin(LF_offset) * x_vel_robot + cos(LF_offset)*y_vel_robot + wheel_radius*theta_vel);
    double LB = -(-sin(LB_offset) * x_vel_robot + cos(LB_offset)*y_vel_robot + wheel_radius*theta_vel);
    double RB =  (-sin(RB_offset) * x_vel_robot + cos(RB_offset)*y_vel_robot + wheel_radius*theta_vel);

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

}

