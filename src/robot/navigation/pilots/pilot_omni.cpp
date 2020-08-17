#include "pilot_omni.h"
#include "gui/interface/gui_interface.h"
#include "robot/robot.h"

#include <cmath>
#include <algorithm>
using std::abs, std::cout, std::endl;


PilotOmni::PilotOmni(Robot* robot,float TRANS_P_K, float TRANS_I_K, float ANGULAR_P_K, float ANGULAR_I_K) :
    Pilot(robot),
    TRANS_P_K(TRANS_P_K),
    TRANS_I_K(TRANS_I_K),
    ANGULAR_P_K(ANGULAR_P_K),
    ANGULAR_I_K(ANGULAR_I_K)
{
}

#define DD if(false)

void PilotOmni::driveTo (Point goalPoint, float theta_goal, Point nextGoalPoint)
{
    // Used to clear accumulated errors if goal changes significantly
    Point rp = *robot;
    double theta_current = robot->getOrientation();
    float angle_to_goal = Measurements::angleBetween(rp, goalPoint);

    DD cout << "---position (" << rp.x << " , "
                   << rp.y << " , "
                   << theta_current << ")"
                   << endl;
    DD cout << "---angle to goal: "  << angle_to_goal << endl;

    distance_error = Measurements::distance(rp,goalPoint);
    angle_error = Measurements::angleDiff(theta_current, theta_goal);

    DD cout << "---errorD: " << distance_error << " , " << dist_error_integral << endl;
    DD cout << "---errorA: " << angle_error    << " , " << angle_error_integral << endl;

    DD cout << endl;

    // Calulate error integral component
    updateErrors(goalPoint);
    prev_goal_target = goalPoint; // didnt make sense to compute this at start, thus moved it here

    DD cout << "---errorD': " << distance_error << " , " << dist_error_integral << endl;
    DD cout << "---errorA': " << angle_error    << " , " << angle_error_integral << endl;

    DD cout << endl;

    DD cout << "---D: pk,pd: " << TRANS_P_K << " , " << TRANS_I_K << endl;
    DD cout << "---D: pk,pd: " << ANGULAR_P_K << " , " << ANGULAR_I_K << endl;

    DD cout << endl;
    // Inertial Frame Velocities

    double linear  =   TRANS_P_K * distance_error +   TRANS_I_K *  dist_error_integral;
    float angular  = ANGULAR_P_K *    angle_error + ANGULAR_I_K * angle_error_integral;
    double x_vel = linear*cos(angle_to_goal);
    double y_vel = linear*sin(angle_to_goal);

    DD cout << "---vels: " << linear << " (" << x_vel << " , " << y_vel << ") " << angular << endl;

    // Adjust angular velocity to traverse the minor turn angle

    if (abs(Measurements::angleDiff(theta_goal,theta_current))<
        abs(Measurements::angleDiff(theta_goal,theta_current+angular)))
        angular=-angular;


    // Reduce speed near target
    bool is_final_target = Measurements::distance(goalPoint, nextGoalPoint) < 0.01;
    if (is_final_target && distance_error < 1000) // with in 1m ?
    {
        x_vel *= 0.4;
        y_vel *= 0.4;
        DD cout << "---within 1m: " << x_vel << "," << y_vel <<endl;
        DD cout << "---vels': " << (linear*0.4) << " (" << x_vel << " , " << y_vel << ") " << angular << endl;
    }



    // Robot Frame Velocities
    double x_vel_robot = cos(theta_current)*x_vel - sin(theta_current)*y_vel;
    double y_vel_robot = sin(theta_current)*x_vel + cos(theta_current)*y_vel;

    DD cout << "---vels'': (" << x_vel_robot << " , " << y_vel_robot << ") " << angular << endl;

//    // Apply acceleration ramp (different for each move type)
//    if (robot->getDribble()){
//        double requested_speed = sqrt(x_vel_robot*x_vel_robot + y_vel_robot * y_vel_robot);
//        if(requested_speed > prev_speed)
//        {
//            x_vel_robot = x_vel_robot * (prev_speed + 2) / requested_speed;
//            y_vel_robot = y_vel_robot * (prev_speed + 2) / requested_speed;
//            requested_speed = prev_speed + 2;
//        }
//        prev_speed = requested_speed;

//        // Cap velocities for dribbling
//        y_vel_robot = fmin(y_vel_robot, DRIBBLE_FRWD_SPD);
//        y_vel_robot = fmax(y_vel_robot, -DRIBBLE_BACK_SPD);

//    } else {
//        const double ACC_PER_FRAME = 1;
//        double speed = robot->getSpeedMillimetersPerFrame();
//        double requested_speed = sqrt(x_vel_robot*x_vel_robot + y_vel_robot*y_vel_robot);
//        if(requested_speed > speed + ACC_PER_FRAME) {
//            x_vel_robot = (speed + ACC_PER_FRAME) * 100 * x_vel_robot/requested_speed;
//            y_vel_robot = (speed + ACC_PER_FRAME) * 100 * y_vel_robot/requested_speed;
//        }
//    }
    setTargetVelocity(Point(x_vel_robot, y_vel_robot), angular);
}

void PilotOmni::normalizeSpeeds(double& LF, double& LB, double& RF, double& RB, double max_mtr_spd) {
    double highest_spd = std::max(fabs(LF), (std::max(fabs(LB), (std::max(fabs(RF), fabs(RB))))));

    if(highest_spd > max_mtr_spd){
        LF*=max_mtr_spd/highest_spd;
        LB*=max_mtr_spd/highest_spd;
        RF*=max_mtr_spd/highest_spd;
        RB*=max_mtr_spd/highest_spd;
    }
}



void PilotOmni::updateErrors(Point goalPoint)
{
    // Reset queues if the target has moved
    if(Measurements::distance(goalPoint, prev_goal_target) > 50)
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

void PilotOmni::clearErrors()
{
    dist_error_deque.clear();
    angle_error_deque.clear();
    dist_error_integral = angle_error_integral = 0;
}


