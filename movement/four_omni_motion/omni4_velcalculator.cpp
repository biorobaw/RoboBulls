#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "utilities/debug.h"
#include "include/config/tolerances.h"
#include "include/config/simulated.h"
using std::abs;

namespace Movement
{

#define FOUR_WHEEL_DEBUG 0

#if SIMULATED
float xy_prop_mult = 0.1;       //Multiplier for Proportional XY
float xy_int_mult = 0;      //Multiplier for integral XY
float theta_prop_mult = 5;    //Multiplier for theta proportional
float theta_int_mult = 0;  //Multiplier for theta integral
#else
float xy_prop_mult = .25;     //Multiplier for Proportional XY
float xy_int_mult = 0;     //Multiplier for integral XY
float theta_prop_mult = 1;    //Multiplier for theta proportional
float theta_int_mult = 0.000;  //Multiplier for theta integral
#endif

FourWheelCalculator::FourWheelCalculator()
{
    //debug::registerVariable("xyp", &xy_prop_mult);
    //debug::registerVariable("xyi", &xy_int_mult);
    //debug::registerVariable("thp", &theta_prop_mult);
    //debug::registerVariable("thi", &theta_int_mult);
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
        return defaultCalc(rob,x_goal,y_goal,theta_goal, moveType);
    }
}

fourWheelVels FourWheelCalculator::defaultCalc
    (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType)
{
    //Current Position
    double x_current = rob->getRobotPosition().x;
    double y_current = rob->getRobotPosition().y;
    double theta_current = rob->getOrientation();
    last_goal_target = Point(x_goal, y_goal);

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_to_goal = Measurments::distance(rp,gp);
    float angle_to_goal = Measurments::angleBetween(rp, gp);
    angle_error = Measurments::angleDiff(rob->getOrientation(), theta_goal);

//    std::cout << "4WheelCalc: " << distance_to_goal << std::endl;


    //Calulate error integral component
    calc_error(x_goal, y_goal);

    // No XY movement when explicitly staying still, and we are close
    // to the target--helps controllers not break
    float xy_prop_used = xy_prop_mult;
    float xy_int_used = xy_int_mult;
    if(moveType == Type::StayStill
            && Measurments::distance(rob, last_goal_target) < ROBOT_RADIUS*1.8) {
        xy_prop_used = 0;
        xy_int_used = 0;
    }

    //Inertial Frame Velocities
    double x_vel =
        (xy_prop_used * distance_to_goal +
         xy_int_used  * dist_error_integral)*cos(angle_to_goal);
    double y_vel =
        (xy_prop_used * distance_to_goal +
         xy_int_used  * dist_error_integral)*sin(angle_to_goal);
    double theta_vel =
         theta_prop_mult * angle_error
       + theta_int_mult  * angle_error_integral;

    if (abs(Measurments::angleDiff(theta_goal,theta_current))<
        abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 700)
    {
        x_vel *= 0.1;
        y_vel *= 0.1;
        theta_vel *= 0.1;
    }

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;
    double vel_robot = sqrt(x_vel_robot*x_vel_robot + y_vel_robot * y_vel_robot);

    // Wheel Velocity Calculations
    double RF =  (-sin(RF_offset) * x_vel_robot + cos(RF_offset)*y_vel_robot - trans_offset*vel_robot*cos(RF_offset) + wheel_radius*theta_vel);
    double LF = -(-sin(LF_offset) * x_vel_robot + cos(LF_offset)*y_vel_robot - trans_offset*vel_robot*cos(LF_offset) + wheel_radius*theta_vel);
    double LB = -(-sin(LB_offset) * x_vel_robot + cos(LB_offset)*y_vel_robot - trans_offset*vel_robot*cos(LB_offset) + wheel_radius*theta_vel);
    double RB =  (-sin(RB_offset) * x_vel_robot + cos(RB_offset)*y_vel_robot - trans_offset*vel_robot*cos(RB_offset) + wheel_radius*theta_vel);

    // Normalize wheel velocities
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
    last_goal_target = Point(x_goal, y_goal);

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_to_goal = Measurments::distance(rp,gp);
    float angle_to_goal = Measurments::angleBetween(rp, gp);
    angle_error = Measurments::angleDiff(rob->getOrientation(), theta_goal);

    //PID
    calc_error(x_goal, y_goal);

    //Unlike defaultCalc, we always use the same XY prop/int multipliers
    float xy_prop_used = xy_prop_mult;
    float xy_int_used = xy_int_mult;

    //Interial Frame Velocities
    double x_vel =
        (xy_prop_used * distance_to_goal +
         xy_int_used  * dist_error_integral)*cos(angle_to_goal);
    double y_vel =
        (xy_prop_used * distance_to_goal +
         xy_int_used  * dist_error_integral)*sin(angle_to_goal);
    double theta_vel =
         theta_prop_mult * angle_error
       + theta_int_mult  * angle_error_integral;

    if (abs(Measurments::angleDiff(theta_goal,theta_current))<
        abs(Measurments::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_to_goal < 700)
    {
        x_vel *= 0.1;
        y_vel *= 0.1;
    }

    // Focus on rotation
    double vel = sqrt(x_vel*x_vel+y_vel*y_vel);
    if (abs(Measurments::angleDiff(theta_goal,theta_current)) > ROT_TOLERANCE*3 && vel > 50)
    {
        x_vel = (abs(x_vel)>=abs(y_vel))? 50 * x_vel/abs(x_vel) : 50*x_vel/abs(y_vel);
        y_vel = (abs(x_vel)<=abs(y_vel))? 50 * y_vel/abs(y_vel) : 50*y_vel/abs(x_vel);
        theta_vel *= 2;
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

void FourWheelCalculator::calc_error(float x_goal, float y_goal)
{
    //Reset queues if the target has moved
    if(Measurments::distance(Point(x_goal,y_goal), last_goal_target) > 50)
        clear_errors();

    //Integral Error for distance
    if (dist_error_deque.size() == dist_error_maxsize) {
        dist_error_integral -= dist_error_deque.front();
        dist_error_deque.pop_front();
    }
    dist_error_integral += distance_to_goal;
    dist_error_deque.push_back(distance_to_goal);

    //Integral Error for orientation
    if (angle_error_deque.size() == angle_error_maxsize) {
        angle_error_integral -= angle_error_deque.front();
        angle_error_deque.pop_front();
    }
    angle_error_integral += angle_error;
    angle_error_deque.push_back(angle_error);
}

void FourWheelCalculator::clear_errors()
{
    dist_error_deque.clear();
    angle_error_deque.clear();
    dist_error_integral = angle_error_integral = 0;
}

}

