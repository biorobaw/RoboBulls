#include "movement/four_wheel_omni/four_wheel_omni_pilot.h"
#include "gui/guiinterface.h"
#include <cmath>
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
/*    // Used to clear accumulated errors if goal changes significantly
    //prev_goal_target = Point(x_goal, y_goal);
    prev_goal_target = Point(0, 0);
    std::cout << "prev_goal_target " << prev_goal_target.toString() << std::endl;
    Point rp = rob->getPosition();
    //Point gp = Point(x_goal,y_goal);
    Point gp = Point(0,0);
    std::cout << "rp: " << rp.toString() << std::endl;
    std::cout << "gp: " << gp.toString() << std::endl;
    distance_error = Measurements::distance(rp,gp);
    float angle_to_goal = Measurements::angleBetween(rp, gp);
    std::cout << "angle_to_goal: " << angle_to_goal << std::endl;
    angle_error = Measurements::angleDiff(rob->getOrientation(), theta_goal);

    // Calulate error integral component
    updateErrors(x_goal, y_goal);

    // Inertial Frame Velocities
    double x_vel = (TRANS_P_K*distance_error + TRANS_I_K*dist_error_integral)*cos(angle_to_goal);
    double y_vel = (TRANS_P_K*distance_error + TRANS_I_K*dist_error_integral)*sin(angle_to_goal);
    double theta_vel = ANGULAR_P_K*angle_error + ANGULAR_I_K*angle_error_integral;

    // Adjust angular velocity to traverse the minor turn angle
    double theta_current = rob->getOrientation();
    if (abs(Measurements::angleDiff(theta_goal,theta_current))<
        abs(Measurements::angleDiff(theta_goal,theta_current+theta_vel)))
        theta_vel=-theta_vel;

    // Reduce speed near target
    bool is_final_target = Measurements::distance(gp, Point(x_goal2, y_goal2)) < 0.01;
    if (is_final_target && distance_error < 1000)
    {
        x_vel *= 0.4;
        y_vel *= 0.4;
    }

    // Robot Frame Velocities
    double y_vel_robot = cos(theta_current)*x_vel+sin(theta_current)*y_vel;
    double x_vel_robot = sin(theta_current)*x_vel-cos(theta_current)*y_vel;

    // Apply acceleration ramp
    const double ACC_PER_FRAME = 1;
    double speed = rob->getSpeedMillimetersPerFrame();
    double requested_speed = sqrt(x_vel_robot*x_vel_robot + y_vel_robot*y_vel_robot);
    if(requested_speed > speed + ACC_PER_FRAME) {
        x_vel_robot = (speed + ACC_PER_FRAME) * 100 * x_vel_robot/requested_speed;
        y_vel_robot = (speed + ACC_PER_FRAME) * 100 * y_vel_robot/requested_speed;
    }

    // Wheel Velocity Calculations
    double RF =  (-sin(RF_OFFSET) * x_vel_robot + cos(RF_OFFSET)*y_vel_robot - TRANS_OFFSET*speed*cos(RF_OFFSET) + WHEEL_RADIUS*theta_vel);
    double LF = -(-sin(LF_OFFSET) * x_vel_robot + cos(LF_OFFSET)*y_vel_robot - TRANS_OFFSET*speed*cos(LF_OFFSET) + WHEEL_RADIUS*theta_vel);
    double LB = -(-sin(LB_OFFSET) * x_vel_robot + cos(LB_OFFSET)*y_vel_robot - TRANS_OFFSET*speed*cos(LB_OFFSET) + WHEEL_RADIUS*theta_vel);
    double RB =  (-sin(RB_OFFSET) * x_vel_robot + cos(RB_OFFSET)*y_vel_robot - TRANS_OFFSET*speed*cos(RB_OFFSET) + WHEEL_RADIUS*theta_vel);

    // Normalize wheel velocities
    normalizeSpeeds(LF, LB, RF, RB, 10);
    std::cout << "DefaultDrive x_vel_robot: " << x_vel_robot<< std::endl;
    std::cout << "DefaultDrive y_vel_robot: " << y_vel_robot<< std::endl;
    if(x_vel_robot > 30)
    {
        x_vel_robot = 10;
    }
    if(x_vel_robot < -30)
    {
        x_vel_robot = -10;
    }
    if(y_vel_robot > 30)
    {
        y_vel_robot = 10;
    }
    if(y_vel_robot < -30)
    {
        y_vel_robot = -10;
    }
    // Set velocities on robot object
    rob->setLF(LF);
    rob->setLB(LB);
    rob->setRF(RF);
    rob->setRB(RB);
    rob->setXVel(x_vel_robot);
    rob->setYVel(y_vel_robot);
    rob ->setAngVel(theta_vel);*/
    //Current Position
    double x_current = rob->getPosition().x;
    double y_current = rob->getPosition().y;
    double theta_current = rob->getOrientation();
    std::cout << "getOrientation: " << theta_current << std::endl;
    double ang_vel_robot = 0;
//    if((theta_current>0.07)||(theta_current<-0.07))
//    {
//        if(theta_current<-0.07)
//        {
//            ang_vel_robot = -20*(-theta_current);
//        }
//        else
//            ang_vel_robot = 20*theta_current;
//    }
    prev_goal_target =  Point(x_goal, y_goal);

    Point rp = Point(x_current,y_current);
    Point gp = Point(x_goal,y_goal);;
    distance_error = Measurements::distance(rp,gp);
    std::cout << "distance_error: " << distance_error << std::endl;
    float angle_to_goal = Measurements::angleBetween(rp, gp);
    std::cout << "angle_to_goal: " << angle_to_goal << std::endl;
    std::cout << "rp: " << rp.toString() << std::endl;
    std::cout << "gp: " << gp.toString() << std::endl;
//    std::cout << "facePointDrive x_vel_robot: " << x_vel_robot<< std::endl;
//    std::cout << "facePointDrive y_vel_robot: " << y_vel_robot<< std::endl;
    // Set velocities on robot object
    // face to point
    angle_error = angle_to_goal - theta_current;
    std::cout << "angle_error: " << angle_error << std::endl;
    double y_vel_robot = 0;
    double x_vel_robot = 1;
    if((angle_error>0.07||angle_error<-0.07))
    {
         ang_vel_robot = -20*(angle_error);
    }else{
        x_vel_robot = x_vel_robot*distance_error;
        if(x_vel_robot > 30)
        {
            x_vel_robot = x_vel_robot *0.1;
        }
        if(x_vel_robot < -30)
        {
            x_vel_robot =  x_vel_robot *0.1;
        }
        if(y_vel_robot > 30)
        {
            y_vel_robot = y_vel_robot*0.1;
        }
        if(y_vel_robot < -30)
        {
            y_vel_robot = y_vel_robot*0.1;
        }
        if(distance_error<30)
        {
          x_vel_robot=0;
        }
    }
    if(x_vel_robot > 30)
    {
        x_vel_robot = 10;
    }
    if(x_vel_robot < -30)
    {
        x_vel_robot = -10;
    }
    if(y_vel_robot > 30)
    {
        y_vel_robot = 10;
    }
    if(y_vel_robot < -30)
    {
        y_vel_robot = -10;
    }
    rob->setXVel(x_vel_robot);
    rob->setYVel(y_vel_robot);
    rob ->setAngVel(ang_vel_robot);
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
    if(vel_robot > prev_speed)
    {
        x_vel_robot = x_vel_robot * (prev_speed + 2) / vel_robot;
        y_vel_robot = y_vel_robot * (prev_speed + 2) / vel_robot;
        vel_robot = prev_speed + 2;
    }
    prev_speed = vel_robot;
    
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
    normalizeSpeeds(LF,LB,RF,RB, 10);//100
    std::cout << "dribbleDrive x_vel_robot: " << x_vel_robot<< std::endl;
    std::cout << "dribbleDrive y_vel_robot: " << y_vel_robot<< std::endl;
    // Set velocities on robot object
    rob->setLF(LF);
    rob->setLB(LB);
    rob->setRF(RF);
    rob->setRB(RB);
    rob->setXVel(x_vel_robot);
    rob->setYVel(y_vel_robot);
    rob ->setAngVel(theta_vel);
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
    normalizeSpeeds(LF, LB, RF, RB, 10);//100
    std::cout << "facePointDrive x_vel_robot: " << x_vel_robot<< std::endl;
    std::cout << "facePointDrive y_vel_robot: " << y_vel_robot<< std::endl;
    // Set velocities on robot object
    rob->setLF(LF);
    rob->setLB(LB);
    rob->setRF(RF);
    rob->setRB(RB);
    rob->setXVel(x_vel_robot);
    rob->setYVel(y_vel_robot);
     rob ->setAngVel(theta_vel);
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

