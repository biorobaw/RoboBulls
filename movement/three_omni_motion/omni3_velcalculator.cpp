#include "movement/three_omni_motion/omni3_velcalculator.h"

namespace Movement
{

threeWheelVels ThreeWheelCalculator::calculateVels
    (Robot* rob, Point goalPoint, float theta_goal, Type moveType)
{
	return calculateVels(rob, goalPoint.x, goalPoint.y, theta_goal, moveType);
}


threeWheelVels ThreeWheelCalculator::calculateVels
    (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType)
{
    switch (moveType)
    {
    case Type::facePoint:
        return facePointCalc(rob,x_goal,y_goal,theta_goal);
    default:
        return defaultCalc(rob,x_goal,y_goal,theta_goal);
    }
}

threeWheelVels ThreeWheelCalculator::defaultCalc
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
    if (distance_to_goal < 400)
    {
        x_vel *= 0.5;
        y_vel *= 0.5;
    }

    // Robot Frame Velocities
    double x_vel_robot =  cos(theta_current)*x_vel + sin(theta_current)*y_vel;
    double y_vel_robot =  sin(theta_current)*x_vel - cos(theta_current)*y_vel;

    // Physical Bias
    std::vector<double> bias = calcBias(x_vel_robot,y_vel_robot);

    //Wheel Velocity Calculations
    double R = -round(-sin(M_PI/6)   * y_vel_robot + cos(M_PI/6)   *x_vel_robot + wheel_radius*theta_vel)*bias[0];
    double L = -round(-sin(5*M_PI/6) * y_vel_robot + cos(5*M_PI/6) *x_vel_robot + wheel_radius*theta_vel)*bias[1];
    double B = -round(-sin(9*M_PI/6) * y_vel_robot + cos(9*M_PI/6) *x_vel_robot + wheel_radius*theta_vel)*bias[2];

    //Normalize wheel velocities
    if (abs(R)>max_mtr_spd)
    {
        L=(max_mtr_spd/abs(R))*L;
        B=(max_mtr_spd/abs(R))*B;
        R=(max_mtr_spd/abs(R))*R;
    }
    if (abs(L)>max_mtr_spd)
    {
        R=(max_mtr_spd/abs(L))*R;
        B=(max_mtr_spd/abs(L))*B;
        L=(max_mtr_spd/abs(L))*L;
    }
    if (abs(B)>max_mtr_spd)
    {
        L=(max_mtr_spd/abs(B))*L;
        R=(max_mtr_spd/abs(B))*R;
        B=(max_mtr_spd/abs(B))*B;
    }

    //Create and return result container
    threeWheelVels vels;
    vels.L = L;
    vels.R = R;
    vels.B = B;
    return vels;
}

threeWheelVels ThreeWheelCalculator::facePointCalc
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
    if (distance_to_goal < 300)
    {
        x_vel *= 0.5;
        y_vel *= 0.5;
    }

    // Focus on rotation
    double vel = sqrt(x_vel*x_vel+y_vel*y_vel);
    if (abs(Measurments::angleDiff(theta_goal,theta_current))>ROT_TOLERANCE*0.5 && vel > 40)
    {
        x_vel = 40*cos(angle_to_goal);
        y_vel = 40*sin(angle_to_goal);
        theta_vel*=2.5;
    }

    // Robot Frame Velocities
    double x_vel_robot = cos(theta_current)*x_vel + sin(theta_current)*y_vel;
    double y_vel_robot = sin(theta_current)*x_vel - cos(theta_current)*y_vel;

    // Physical Bias
    std::vector<double> bias = calcBias(x_vel_robot,y_vel_robot);

    //Wheel Velocity Calculations
    double R = -round(-sin(M_PI/6)   * y_vel_robot + cos(M_PI/6)   *x_vel_robot + wheel_radius*theta_vel)*bias[0];
    double L = -round(-sin(5*M_PI/6) * y_vel_robot + cos(5*M_PI/6) *x_vel_robot + wheel_radius*theta_vel)*bias[1];
    double B = -round(-sin(9*M_PI/6) * y_vel_robot + cos(9*M_PI/6) *x_vel_robot + wheel_radius*theta_vel)*bias[2];

    //Normalize wheel velocities
    if (abs(R)>max_mtr_spd)
    {
        L=(max_mtr_spd/abs(R))*L;
        B=(max_mtr_spd/abs(R))*B;
        R=(max_mtr_spd/abs(R))*R;
    }
    if (abs(L)>max_mtr_spd)
    {
        R=(max_mtr_spd/abs(L))*R;
        B=(max_mtr_spd/abs(L))*B;
        L=(max_mtr_spd/abs(L))*L;
    }
    if (abs(B)>max_mtr_spd)
    {
        L=(max_mtr_spd/abs(B))*L;
        R=(max_mtr_spd/abs(B))*R;
        B=(max_mtr_spd/abs(B))*B;
    }

    //Create and return result container
    threeWheelVels vels;
    vels.L = L;
    vels.R = R;
    vels.B = B;
    return vels;
}

std::vector<double> ThreeWheelCalculator::calcBias(double x, double y)
{
    std::vector<double> bias_result;
    bias_result.push_back(1);
    bias_result.push_back(2);
    bias_result.push_back(3);

    if (abs(x)>90)
    {
        bias_result[0] = 1;
        bias_result[1] = 0.90;
        bias_result[2] = 1;
    }
    else if (abs(x)>80)
    {
        bias_result[0] = 1;
        bias_result[1] = 0.97;
        bias_result[2] = 1;
    }

    if (abs(y)>90)
    {
        bias_result[0] = 1;
        bias_result[1] = 1;
        bias_result[2] = 1.3;
    }
    else if (abs(y)>80)
    {
        bias_result[0] = 1;
        bias_result[1] = 1;
        bias_result[2] = 1.35;
    }
    else if (abs(y)>70)
    {
        bias_result[0] = 1;
        bias_result[1] = 1;
        bias_result[2] = 1.33;
    }


    return bias_result;
}
}
