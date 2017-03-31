#include "movement/four_wheel_omni/four_wheel_omni_pilot.h"
#include "gui/guiinterface.h"
using std::abs;

namespace Move {

FourWheelOmniPilot::FourWheelOmniPilot()
{
}

void FourWheelOmniPilot::drive (Robot* rob, float x_goal, float y_goal, float theta_goal, float x_goal2, float y_goal2, MoveType moveType){
    switch (moveType)
    {
    case MoveType::facePoint:
        facePointDrive(rob,x_goal,y_goal,theta_goal);
        break;
    case MoveType::dribble:
        dribbleDrive(rob,x_goal,y_goal,theta_goal);
        break;
    default:
        defaultDrive(rob,x_goal,y_goal,theta_goal, x_goal2, y_goal2);
    }
}

void FourWheelOmniPilot::drive (Robot* rob, Point goalPoint, float theta_goal, Point goal2Point, MoveType moveType)
{
    drive(rob, goalPoint.x, goalPoint.y, theta_goal, goal2Point.x, goal2Point.y, moveType);
}

void FourWheelOmniPilot::normalizeSpeeds(double& LF, double& LB, double& RF, double& RB, double max_mtr_spd) {
    double highest_spd = std::max(fabs(LF), (std::max(fabs(LB), (std::max(fabs(RF), fabs(RB))))));

    if(highest_spd > max_mtr_spd){
        LF*=max_mtr_spd/highest_spd;
        LB*=max_mtr_spd/highest_spd;
        RF*=max_mtr_spd/highest_spd;
        RB*=max_mtr_spd/highest_spd;
    }
}

void FourWheelOmniPilot::defaultDrive (Robot* rob, float x_goal, float y_goal, float theta_goal, float x_goal2, float y_goal2)
{
    // Used to clear accumulated errors if goal changes significantly
    prev_goal_target = Point(x_goal, y_goal);

    Point rp = rob->getPosition();
    Point gp = Point(x_goal,y_goal);
    distance_error = Measurements::distance(rp,gp);
    float angle_to_goal = Measurements::angleBetween(rp, gp);
    angle_error = Measurements::angleDiff(rob->getOrientation(), theta_goal);

    //Calulate error integral component
    updateErrors(x_goal, y_goal);

    //Inertial Frame Velocities
    double x_vel =
        (TRANS_P_K * distance_error +
         TRANS_I_K  * dist_error_integral)*cos(angle_to_goal);
    double y_vel =
        (TRANS_P_K * distance_error +
         TRANS_I_K  * dist_error_integral)*sin(angle_to_goal);
    double theta_vel =
         ANGULAR_P_K * angle_error +
         ANGULAR_I_K  * angle_error_integral;

    double theta_current = rob->getOrientation();
    if (abs(Measurements::angleDiff(theta_goal,theta_current))<
        abs(Measurements::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    bool is_final_target = Measurements::distance(gp, Point(x_goal2, y_goal2)) < 0.01;
    if (is_final_target && distance_error < 700)
    {
        x_vel *= 0.5;
        y_vel *= 0.5;
        theta_vel *= 0.5;
    }

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;
    double vel_robot = sqrt(x_vel_robot*x_vel_robot + y_vel_robot * y_vel_robot);

    // Apply acceleration ramp
    if(vel_robot > prev_vel)
    {
        x_vel_robot = x_vel_robot * (prev_vel + 2) / vel_robot;
        y_vel_robot = y_vel_robot * (prev_vel + 2) / vel_robot;
        vel_robot = prev_vel + 0.7;
    }
    prev_vel = vel_robot;

    // Wheel Velocity Calculations
    double RF =  (-sin(RF_OFFSET) * x_vel_robot + cos(RF_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(RF_OFFSET) + WHEEL_RADIUS*theta_vel);
    double LF = -(-sin(LF_OFFSET) * x_vel_robot + cos(LF_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(LF_OFFSET) + WHEEL_RADIUS*theta_vel);
    double LB = -(-sin(LB_OFFSET) * x_vel_robot + cos(LB_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(LB_OFFSET) + WHEEL_RADIUS*theta_vel);
    double RB =  (-sin(RB_OFFSET) * x_vel_robot + cos(RB_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(RB_OFFSET) + WHEEL_RADIUS*theta_vel);

    // Normalize wheel velocities
    normalizeSpeeds(LF, LB, RF, RB, 100);

    // Set velocities on robot object
    rob->setLF(LF);
    rob->setLB(LB);
    rob->setRF(RF);
    rob->setRB(RB);
}

void FourWheelOmniPilot::dribbleDrive
    (Robot* rob, float x_goal, float y_goal, float theta_goal)
{
    //Current Position
    double x_current = rob->getPosition().x;
    double y_current = rob->getPosition().y;
    double theta_current = rob->getOrientation();
    prev_goal_target = Point(x_goal, y_goal);

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_error = Measurements::distance(rp,gp);
    float angle_to_goal = Measurements::angleBetween(rp, gp);
    angle_error = Measurements::angleDiff(rob->getOrientation(), theta_goal);

    //Calulate error integral component
    updateErrors(x_goal, y_goal);

    //Inertial Frame Velocities
    double x_vel =
        (TRANS_P_K * distance_error +
         TRANS_I_K  * dist_error_integral)*cos(angle_to_goal);
    double y_vel =
        (TRANS_P_K * distance_error +
         TRANS_I_K  * dist_error_integral)*sin(angle_to_goal);
    double theta_vel =
         ANGULAR_P_K * angle_error +
         ANGULAR_I_K  * angle_error_integral;

    if (abs(Measurements::angleDiff(theta_goal,theta_current))<
        abs(Measurements::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;
    double vel_robot = sqrt(x_vel_robot*x_vel_robot + y_vel_robot * y_vel_robot);

    // Apply acceleration ramp
    if(vel_robot > prev_vel)
    {
        x_vel_robot = x_vel_robot * (prev_vel + 2) / vel_robot;
        y_vel_robot = y_vel_robot * (prev_vel + 2) / vel_robot;
        vel_robot = prev_vel + 2;
    }
    prev_vel = vel_robot;
    
    // Cap velocities for dribbling
    y_vel_robot = fmin(y_vel_robot, DRIBBLE_FRWD_SPD);
    y_vel_robot = fmax(y_vel_robot, -DRIBBLE_BACK_SPD);

    theta_vel -= x_vel_robot;
    theta_vel = fmin(theta_vel, DRIBBLE_TURN_RATE);
    theta_vel = fmax(theta_vel, -DRIBBLE_TURN_RATE);

    x_vel_robot = 0;

    // Wheel Velocity Calculations
    double RF =  (-sin(RF_OFFSET) * x_vel_robot + cos(RF_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(RF_OFFSET) + WHEEL_RADIUS*theta_vel);
    double LF = -(-sin(LF_OFFSET) * x_vel_robot + cos(LF_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(LF_OFFSET) + WHEEL_RADIUS*theta_vel);
    double LB = -(-sin(LB_OFFSET) * x_vel_robot + cos(LB_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(LB_OFFSET) + WHEEL_RADIUS*theta_vel);
    double RB =  (-sin(RB_OFFSET) * x_vel_robot + cos(RB_OFFSET)*y_vel_robot - TRANS_OFFSET*vel_robot*cos(RB_OFFSET) + WHEEL_RADIUS*theta_vel);

    // Normalize wheel velocities
    normalizeSpeeds(LF,LB,RF,RB, 100);

    // Set velocities on robot object
    rob->setLF(LF);
    rob->setLB(LB);
    rob->setRF(RF);
    rob->setRB(RB);
}

void FourWheelOmniPilot::facePointDrive(Robot* rob, float x_goal, float y_goal, float theta_goal)
{
    //Current Position
    double x_current = rob->getPosition().x;
    double y_current = rob->getPosition().y;
    double theta_current = rob->getOrientation();
    prev_goal_target = Point(x_goal, y_goal);

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);
    distance_error = Measurements::distance(rp,gp);
    float angle_to_goal = Measurements::angleBetween(rp, gp);
    angle_error = Measurements::angleDiff(rob->getOrientation(), theta_goal);

    // PID
    updateErrors(x_goal, y_goal);

    // Unlike defaultCalc, we always use the same XY prop/int multipliers
    float xy_prop_used = TRANS_P_K;
    float xy_int_used = TRANS_I_K;

    //Interial Frame Velocities
    double x_vel =
        (xy_prop_used * distance_error +
         xy_int_used  * dist_error_integral)*cos(angle_to_goal);
    double y_vel =
        (xy_prop_used * distance_error +
         xy_int_used  * dist_error_integral)*sin(angle_to_goal);
    double theta_vel =
         ANGULAR_P_K * angle_error
       + ANGULAR_I_K  * angle_error_integral;

    if (abs(Measurements::angleDiff(theta_goal,theta_current))<
        abs(Measurements::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    if (distance_error < 700)
    {
        x_vel *= 0.1;
        y_vel *= 0.1;
    }

    // Focus on rotation
    double vel = sqrt(x_vel*x_vel+y_vel*y_vel);
    if (abs(Measurements::angleDiff(theta_goal,theta_current)) > ROT_TOLERANCE*3 && vel > 50)
    {
        x_vel = (abs(x_vel)>=abs(y_vel))? 50 * x_vel/abs(x_vel) : 50*x_vel/abs(y_vel);
        y_vel = (abs(x_vel)<=abs(y_vel))? 50 * y_vel/abs(y_vel) : 50*y_vel/abs(x_vel);
        theta_vel *= 2;
    }

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;

    // Wheel Velocity Calculations
    double RF =  (-sin(RF_OFFSET) * x_vel_robot + cos(RF_OFFSET)*y_vel_robot + WHEEL_RADIUS*theta_vel);
    double LF = -(-sin(LF_OFFSET) * x_vel_robot + cos(LF_OFFSET)*y_vel_robot + WHEEL_RADIUS*theta_vel);
    double LB = -(-sin(LB_OFFSET) * x_vel_robot + cos(LB_OFFSET)*y_vel_robot + WHEEL_RADIUS*theta_vel);
    double RB =  (-sin(RB_OFFSET) * x_vel_robot + cos(RB_OFFSET)*y_vel_robot + WHEEL_RADIUS*theta_vel);

    // Normalize wheel velocities
    normalizeSpeeds(LF,LB,RF,RB, 100);

    // Set velocities on robot object
    rob->setLF(LF);
    rob->setLB(LB);
    rob->setRF(RF);
    rob->setRB(RB);
}

void FourWheelOmniPilot::updateErrors(float x_goal, float y_goal)
{
    // Reset queues if the target has moved
    if(Measurements::distance(Point(x_goal,y_goal), prev_goal_target) > 50)
        clearErrors();

    //Integral Error for distance
    if (dist_error_deque.size() == DIST_ERROR_MAXSIZE) {
        double popped_integral = dist_error_integral - dist_error_deque.front();
        double sign_popped_integral = (popped_integral > 0) - (popped_integral < 0);
        dist_error_integral = std::min(1000000.0, fabs(popped_integral)) * sign_popped_integral;
        dist_error_deque.pop_front();
    }
    double pushed_integral = dist_error_integral + distance_error;
    double sign_pushed_integral = (pushed_integral > 0) - (pushed_integral < 0);
    dist_error_integral = std::min(1000000.0, fabs(pushed_integral)) * sign_pushed_integral;
    dist_error_deque.push_back(distance_error);

    //Integral Error for orientation
    if (angle_error_deque.size() == ANGLE_ERROR_MAXSIZE) {
        angle_error_integral -= angle_error_deque.front();
        angle_error_deque.pop_front();
    }
    angle_error_integral += angle_error;
    angle_error_deque.push_back(angle_error);

    //std::cout << "DIST ERR INT: " << dist_error_integral << std::endl;
}

void FourWheelOmniPilot::clearErrors()
{
    dist_error_deque.clear();
    angle_error_deque.clear();
    dist_error_integral = angle_error_integral = 0;
}

}   // namespace Move

