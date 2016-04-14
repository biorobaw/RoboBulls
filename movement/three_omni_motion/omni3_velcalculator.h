#ifndef THREEWHEEL_VELCALCULATOR_H
#define THREEWHEEL_VELCALCULATOR_H
#include <cmath>
#include <vector>
#include "model/robot.h"
#include "movement/movetype.h"
#include "utilities/measurments.h"

namespace Movement
{

//! @brief Return value from ThreeWheelCalculator
struct threeWheelVels
{
    int L, R, B;
};

/*! @brief Wheel velocity calculator for three-wheel robots
 * @author Muhaimen Shamsi
 * @details This class exists to provide wheel velocities for a three-wheeled robot,
 * where each wheel is approximately 33 degrees apart. Currently, this is used for
 * thre three-wheeled lego goalie Robots at RoboBulls.
 * @see FourWheelCalculator
 * @see DifferentialCalculator */
class ThreeWheelCalculator
{
public: 
    /*! @brief Velocity calculation with a Point target overload
     * @param goalPoint The target point
     * @see calculateVels */
	threeWheelVels calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);

    /*! @brief Calculate an instantanious threeWheelVels to move torwards a point
     * @param rob Robot to calculate for
     * @param x_goal The X posiiton of the final target
     * @param y_goal the Y position of the final taget
     * @param theta_goal The desired ending facing angle angle
     * @param moveType The Movement Type. See Type
     * @return A threeWheelVels to be sent to the robot's left, right, and back wheels */
	threeWheelVels calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType);

private:
    const double wheel_radius = 27;
    double max_mtr_spd = 100.0;
    double distance_to_goal, angle_to_goal;

    //! @brief Movement algorithm to handle Type::Default movement
    threeWheelVels defaultCalc(Robot* rob, float x_goal, float y_goal, float theta_goal);

    //! @brief Movement algorithm to handle Type::facePoint movement
    threeWheelVels facePointCalc(Robot* rob, float x_goal, float y_goal, float theta_goal);

    //! @brief Approximates bias adjustment values incurred with three-wheel movement
    std::vector<double> calcBias(double x, double y);
};

}

#endif
