#ifndef FOURWHEEL_VELCALCULATOR_H
#define FOURWHEEL_VELCALCULATOR_H
#include <math.h>
#include <deque>
#include <iostream>
#include "model/robot.h"
#include "movement/movetype.h"
#include "utilities/measurements.h"
#include "utilities/debug.h"
#include "include/config/tolerances.h"
#include "include/config/simulated.h"

namespace Movement
{

//! @brief Return value from FourWheelCalculator
struct fourWheelVels
{
    double LB, LF, RB, RF;
};

/*! @brief Wheel velocity calculator for four-wheel (Ardino) robots
 * @author Muhaimen Shamsi
 * @details This class exists to provide wheel velocities for a the main
 * RoboBulls arsenal of four-wheel robots, or the standard Robocup SSL robot.
 * @see ThreeWheelCalculator
 * @see DifferentialCalculator */
class FourWheelCalculator
{
public:
    FourWheelCalculator();

    /*! @brief Velocity calculation with a Point target overload
     * @param goalPoint The target point
     * @see calculateVels */
	fourWheelVels calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);

    /*! @brief Calculate an instantanious threeWheelVels to move torwards a point
     * @param rob Robot to calculate for
     * @param x_goal The X posiiton of the final target
     * @param y_goal the Y position of the final taget
     * @param theta_goal The desired ending facing angle angle
     * @param moveType The Movement Type. See Type
     * @return a fourWheelVels to be sent to the robot's LB, LF, RB, and RF wheels */
	fourWheelVels calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Movement::Type move_type);

private:
    //! @brief Movement algorithm to handle Type::Default and Type::StayStill movement
    fourWheelVels defaultCalc(Robot* rob, float x_goal, float y_goal, float theta_goal);

    //! @brief Movement algorithm to handle Type::facePoint movement
    fourWheelVels facePointCalc(Robot* rob, float x_goal, float y_goal, float angle_to_point);

    //Robot Physical Properties
    double LF_offset = 144*M_PI/180; //135 Robot's x-Axis (right side) is zero
    double LB_offset = 224*M_PI/180; //225
    double RF_offset =  36*M_PI/180; //45
    double RB_offset = 316*M_PI/180; //315
#if SIMULATED
    double trans_offset = 0;
#else
    double trans_offset = 0.0149;
#endif
    const double wheel_radius = 27;
    double distance_to_goal, angle_error;

    //Error Memebers
    void calc_error(float x_goal, float y_goal);
    void clear_errors();
    Point last_goal_target;
    unsigned int dist_error_maxsize=100;
    unsigned int angle_error_maxsize=50;
    double dist_error_integral=0;
    double angle_error_integral=0;
    std::deque<double> dist_error_deque;
    std::deque<double> angle_error_deque;
};

}

#endif
