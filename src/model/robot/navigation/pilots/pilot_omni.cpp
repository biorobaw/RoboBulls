#include "pilot_omni.h"
#include "gui/interface/gui_interface.h"
#include "model/robot/robot.h"

#include <cmath>
#include <algorithm>
#include <QDebug>
#include <QDateTime>
using std::abs;


PilotOmni::PilotOmni(Robot* robot,float TRANS_P_K, float TRANS_I_K, float ANGULAR_P_K, float ANGULAR_I_K) :
    RobotPilot(robot)
//    TRANS_P_K(TRANS_P_K),
//    TRANS_I_K(TRANS_I_K),
//    ANGULAR_P_K(ANGULAR_P_K),
//    ANGULAR_I_K(ANGULAR_I_K)
{
}

#define DD if(false)

void PilotOmni::driveTo (Point goalPoint, float theta_goal, Point nextGoalPoint)
{

    // calculate time since last control cycle
    auto new_time = robot->getTimeStamp();
    auto delta_t = new_time-time_stamp;
    time_stamp = new_time;
    if(delta_t > 0.300) delta_t = 0.100; // this will be true if the function hasn't been used in a while
//    qDebug() << "delta:" << delta_t;


    // TODO: change the following definitions into parameters
#define MAX_ACC   2500
#define MAX_DEACC  800
#define MAX_SPEED 1500

#define MAX_ACC_ANG   1500/180.0*3.14
#define MAX_DEACC_ANG  100/180.0*3.14
#define MAX_ANGULAR     80/180.0*3.14
#define ANG_TOLERANCE    4/180.0*3.14



    auto robot_vel   = robot->getVelocity();
    if(robot_vel.norm() > 1.5*MAX_SPEED){
        // this is an error due to "teleporting to robot", skip this cycle
        return;
    }

    // ==== Calculate angular speed ================================

    float orientation = robot->getOrientation();
    float angular     = robot->getAngularSpeed();
    float orientationError = Measurements::angleDiff(theta_goal, orientation);
    int sign = orientationError > 0 ? 1 : -1;

    float target_angular = sqrt(2*MAX_DEACC_ANG*fmax(abs(orientationError)-ANG_TOLERANCE,0));
    target_angular       = sign*fmin(target_angular, MAX_ANGULAR);
    float angular_error  = target_angular - robot->getAngularSpeed();
    float angular_norm   = fmax(abs(angular_error), 0.001);
    float angular_learn_rate = fmin(MAX_ACC_ANG*delta_t/angular_norm, 1);

    float new_angular = angular + angular_error*angular_learn_rate;
    float abs_angular = abs(angular);
    if(abs_angular > MAX_ANGULAR) new_angular = sign*MAX_ANGULAR;
    abs_angular = abs(new_angular);


    // =============================================================

    Point pos_estimation    = *robot;// + robot->getVelocity()*delta_t;
    auto  position_error    = goalPoint - pos_estimation;
    auto  distance_to_goal  = position_error.norm();
    float angle_to_goal     = position_error.angle();

    auto delta_theta = new_angular * delta_t; //+angular)/2 * delta_t;
    double target_angle = angle_to_goal - delta_theta/2; // correct for spin

    //auto target_speed = sqrt(2*MAX_DEACC*fmax(distance_to_goal-100,0));
    auto target_speed = sqrt(2*MAX_DEACC*fmax(distance_to_goal-DIST_TOLERANCE,0));// max speed for target distance based on max deacc
    target_speed      = fmin(target_speed, MAX_SPEED);

    auto target_vel   = Point(target_angle)*target_speed;     // ideal velocity (in global coordinates)


    auto error       = target_vel - robot_vel;
    auto error_norm  = fmax(error.norm(),1);
    auto learn_rate  = fmin(MAX_ACC*delta_t/error_norm, 1);


    auto new_vel = robot_vel + error*learn_rate;

    float new_vel_norm = new_vel.norm();
    if(new_vel_norm > MAX_SPEED) new_vel *= (MAX_SPEED / new_vel_norm);



    qInfo() <<"New velocity: lin, ang"<< new_vel <<", "<<new_angular;
    qInfo() << "Goal Point: " <<goalPoint;
    qInfo()<<"Distance to goal: " <<distance_to_goal <<" \t learn rate: " <<learn_rate << "\t error: " << error;
    robot->setTargetVelocityGlobal(new_vel ,new_angular);// new_angular);


}

//void PilotOmni::normalizeSpeeds(double& LF, double& LB, double& RF, double& RB, double max_mtr_spd) {
//    double highest_spd = std::max(fabs(LF), (std::max(fabs(LB), (std::max(fabs(RF), fabs(RB))))));

//    if(highest_spd > max_mtr_spd){
//        LF*=max_mtr_spd/highest_spd;
//        LB*=max_mtr_spd/highest_spd;
//        RF*=max_mtr_spd/highest_spd;
//        RB*=max_mtr_spd/highest_spd;
//    }
//}



//void PilotOmni::updateErrors(Point goalPoint)
//{
//    // NOTE: integral error integrates only a window of time

//    // Reset queues if the target has moved
//    if(Measurements::distance(goalPoint, prev_goal_target) > 50)
//        clearErrors();

//    // increase integral error and append new error to buffer
//    dist_error_integral += distance_error;
//    dist_error_deque.push_back(distance_error);

//    // if buffer overflowed, remove first element
//    if (dist_error_deque.size() == DIST_ERROR_MAXSIZE) {
//        dist_error_integral -= dist_error_deque.front(); // integral of only last N times
//        dist_error_deque.pop_front();
//    }


//    // threshold the integral error
//    if     (dist_error_integral < -1000000) dist_error_integral = -1000000;
//    else if(dist_error_integral > 1000000 ) dist_error_integral =  1000000;


//    //Integral Error for orientation,  same steps:
//    angle_error_integral += angle_error;
//    angle_error_deque.push_back(angle_error);

//    if (angle_error_deque.size() == ANGLE_ERROR_MAXSIZE) {
//        angle_error_integral -= angle_error_deque.front();
//        angle_error_deque.pop_front();
//    }

//    //std::cout << "DIST ERR INT: " << dist_error_integral << std::endl;
//}

//void PilotOmni::clearErrors()
//{
//    dist_error_deque.clear();
//    angle_error_deque.clear();
//    dist_error_integral = angle_error_integral = 0;
//}


