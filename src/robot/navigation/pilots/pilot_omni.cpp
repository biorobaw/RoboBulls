#include "pilot_omni.h"
#include "gui/interface/gui_interface.h"
#include "robot/robot.h"

#include <cmath>
#include <algorithm>
#include <QDebug>
#include <QDateTime>
using std::abs;


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
//    static qint64 time_stamp = robot->getTimeStamp();
//    auto new_time = robot->getTimeStamp();
//    auto delta_t = new_time-time_stamp;
//    qDebug() << "Delta T: " << delta_t;
//    time_stamp = new_time;
//    if(delta_t > 0.200) delta_t = 0.100; // if delta_T is too big, its been a while since this loop was executed, set to averge

    // Used to clear accumulated errors if goal changes significantly
    double theta_current = robot->getOrientation();

    auto error_vector   = goalPoint - *robot;
    float angle_to_goal = error_vector.angle();
    distance_error      = error_vector.norm();
    angle_error         = Measurements::angleDiff(theta_current, theta_goal);

    DD qDebug() << "---position: " << *robot << theta_current;
    DD qDebug() << "---error_T:  " << error_vector  << angle_to_goal ;
    DD qDebug() << "---errorD:   " << distance_error << " , " << dist_error_integral;
//    DD qDebug() << "---errorA: " << angle_error    << " , " << angle_error_integral;

    // Calulate error integral component
    updateErrors(goalPoint);
    prev_goal_target = goalPoint; // didnt make sense to compute this at start, thus moved it here

    DD qDebug() << "---D: t pk,pi: " << TRANS_P_K   << " , " << TRANS_I_K;
    DD qDebug() << "---D: a pk,pi: " << ANGULAR_P_K << " , " << ANGULAR_I_K;

    DD qDebug() << "---errorD': " << distance_error << " , " << dist_error_integral;
//    DD qDebug() << "---errorA': " << angle_error    << " , " << angle_error_integral << endl;



    // Inertial Frame Velocities

#define K 1500.0
    double linear  =   (K/800) * distance_error +   TRANS_I_K *  dist_error_integral;
    float angular  = ANGULAR_P_K *    angle_error + ANGULAR_I_K * angle_error_integral;


    DD qDebug() << "---linear(1), angular: " << linear << angular;

    // Reduce speed near target
//    bool is_final_target = (goalPoint - nextGoalPoint).norm() < 10; // next goal is 1 cm from final goal
//    if (is_final_target && distance_error < 1000) // with in 1m ?
//    {
//        linear *= 0.4;
//        DD qDebug() << "---linear(2): " << linear;
//    }

    if(linear > K) linear = K;
#define TRANS_D_K 0.5
    // calculate velocity in each direction from robot reference frame
    auto target_vel = Point(angle_to_goal-theta_goal)*linear - robot->getVelocity()*TRANS_D_K;


    //target_vel = target_vel*0.5 + error_vel*0.5;


    DD qDebug() << "---vel: " << target_vel;


    // Adjust angular velocity to traverse the minor turn angle
    if (abs(Measurements::angleDiff(theta_goal,theta_current))<
        abs(Measurements::angleDiff(theta_goal,theta_current+angular)))
        angular=-angular;


    DD qDebug() << "---final'': " << target_vel << angular << endl;

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
    angular = 0;
    setTargetVelocity(target_vel, angular);
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
    // NOTE: integral error integrates only a window of time

    // Reset queues if the target has moved
    if(Measurements::distance(goalPoint, prev_goal_target) > 50)
        clearErrors();

    // increase integral error and append new error to buffer
    dist_error_integral += distance_error;
    dist_error_deque.push_back(distance_error);

    // if buffer overflowed, remove first element
    if (dist_error_deque.size() == DIST_ERROR_MAXSIZE) {
        dist_error_integral -= dist_error_deque.front(); // integral of only last N times
        dist_error_deque.pop_front();
    }


    // threshold the integral error
    if     (dist_error_integral < -1000000) dist_error_integral = -1000000;
    else if(dist_error_integral > 1000000 ) dist_error_integral =  1000000;


    //Integral Error for orientation,  same steps:
    angle_error_integral += angle_error;
    angle_error_deque.push_back(angle_error);

    if (angle_error_deque.size() == ANGLE_ERROR_MAXSIZE) {
        angle_error_integral -= angle_error_deque.front();
        angle_error_deque.pop_front();
    }

    //std::cout << "DIST ERR INT: " << dist_error_integral << std::endl;
}

void PilotOmni::clearErrors()
{
    dist_error_deque.clear();
    angle_error_deque.clear();
    dist_error_integral = angle_error_integral = 0;
}


