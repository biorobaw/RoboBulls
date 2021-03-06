#ifndef FOUR_WHEEL_OMNI_PILOT_H
#define FOUR_WHEEL_OMNI_PILOT_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <limits>
#include <deque>
#include <iostream>
#include "utilities/measurements.h"
#include "utilities/debug.h"
#include "../robot_pilot.h"


class PilotOmni : public RobotPilot
{
public:
    PilotOmni(Robot* robot,float TRANS_P_K, float TRANS_I_K, float ANGULAR_P_K, float ANGULAR_I_K);

    /*! @brief Drive function with a Point target overload
     * @param goalPoint The target point
     * @see calculateVels */
    void driveTo (Point goalPoint, float theta_goal, Point nextGoalPoint) override;


private:
    


    //! @brief Normalizes input speeds within -max_mtr_spd and max_mtr_spd
//    void normalizeSpeeds(double& vx, double& LB, double& RF, double& RB, double max_mtr_spd);

//    double TRANS_P_K = 0.25;       // Multiplier for Proportional XY
//    double TRANS_I_K = 0;          // Multiplier for integral XY
//    double ANGULAR_P_K = 0.1;      // Multiplier for theta proportional
//    double ANGULAR_I_K = 0;        // Multiplier for theta integral

//    double distance_error = 0, angle_error =0;

//    const unsigned int DIST_ERROR_MAXSIZE = 100;
//    const unsigned int ANGLE_ERROR_MAXSIZE = 50;
//    double dist_error_integral = 0;
//    double angle_error_integral = 0;
//    std::deque<double> dist_error_deque;
//    std::deque<double> angle_error_deque;

//    void updateErrors(Point goalPoint);
//    void clearErrors();

//    Point prev_goal_target = Point(15000,15000); // a very far away point
//    float prev_speed = 0;


    qint64 time_stamp = 0;
};



#endif
