#ifndef FOUR_WHEEL_OMNI_PILOT_H
#define FOUR_WHEEL_OMNI_PILOT_H
#include <math.h>
#include <limits>
#include <deque>
#include <iostream>
#include "model/robot.h"
#include "movement/movetype.h"
#include "utilities/measurements.h"
#include "utilities/debug.h"
#include "include/motion_parameters.h"

namespace Move
{

/*! @brief Wheel velocity calculator for four-wheel (Ardino) robots
 * @author Muhaimen Shamsi
 * @details This class exists to provide wheel velocities for a the main
 * RoboBulls arsenal of four-wheel robots, or the standard Robocup SSL robot.
 * @see ThreeWheelCalculator
 * @see DifferentialCalculator */
class FourWheelOmniPilot
{
public:
    FourWheelOmniPilot();

    /*! @brief Calculate an instantanious threeWheelVels to move torwards a point
     * @param rob Robot to calculate for
     * @param x_goal The X posiiton of the final target
     * @param y_goal the Y position of the final taget
     * @param theta_goal The desired ending facing angle angle
     * @param moveType The Movement Type. See Type
     * @return a fourWheelVels to be sent to the robot's LB, LF, RB, and RF wheels */
    void drive (Robot* rob, float x_goal, float y_goal, float theta_goal,
                float x_goal2, float y_goal2, MoveType move_type);

    /*! @brief Drive function with a Point target overload
     * @param goalPoint The target point
     * @see calculateVels */
    void drive (Robot* rob, Point goalPoint, float theta_goal,
                Point goalPoint2, MoveType moveType);

private:
    //! @brief Movement algorithm to handle Type::Default
    void defaultDrive(Robot* rob, float x_goal, float y_goal, float theta_goal,
                      float x_goal2, float y_goal2);
    
    //! @brief Movement algorithm to handle Type::Dribble movement
    void dribbleDrive(Robot* rob, float x_goal, float y_goal, float theta_goal);

    //! @brief Movement algorithm to handle Type::facePoint movement
    void facePointDrive(Robot* rob, float x_goal, float y_goal, float angle_to_point);

    //! @brief Normalizes input speeds within -max_mtr_spd and max_mtr_spd
    void normalizeSpeeds(double& LF, double& LB, double& RF, double& RB, double max_mtr_spd);

    // Robot Physical Properties
    static constexpr double LF_OFFSET = 144*M_PI/180; //135 (Robot's x-Axis/right side is zero)
    static constexpr double LB_OFFSET = 224*M_PI/180; //225
    static constexpr double RF_OFFSET =  36*M_PI/180; //45
    static constexpr double RB_OFFSET = 316*M_PI/180; //315

    // Real robots are not physically symmetrical so the following
    // offset value is needed when calculating velocities
    //TODO: code is robot dependent + SIMULATED is no longer a compiler tag
//    # if SIMULATED
//        const double TRANS_OFFSET = 0;
//    # else
        const double TRANS_OFFSET = 0.0149;
//    # endif

    const double WHEEL_RADIUS = 27;

    // PID Error Variables
    //TODO: this should depend on robot not on simulated or not
//    # if SIMULATED
//        const double TRANS_P_K = 1/7.0;       // Multiplier for Proportional XY
//        const double TRANS_I_K = 0.00000;    // Multiplier for integral XY
//        const double ANGULAR_P_K = 0.5;      // Multiplier for theta proportional
//        const double ANGULAR_I_K = 0.0015;   // Multiplier for theta integral
//    # else
        const float PID_DIST = 500;
        const double TRANS_P_K = 0.25;       // Multiplier for Proportional XY
        const double TRANS_I_K = 0;    // Multiplier for integral XY
        const double ANGULAR_P_K = 0.1;      // Multiplier for theta proportional
        const double ANGULAR_I_K = 0;   // Multiplier for theta integral
//    # endif

    double distance_error, angle_error;

    const uint DIST_ERROR_MAXSIZE = 100;
    const uint ANGLE_ERROR_MAXSIZE = 50;
    double dist_error_integral = 0;
    double angle_error_integral = 0;
    std::deque<double> dist_error_deque;
    std::deque<double> angle_error_deque;

    void updateErrors(float x_goal, float y_goal);
    void clearErrors();

    Point prev_goal_target;
    float prev_speed = 0;
};

}

#endif
