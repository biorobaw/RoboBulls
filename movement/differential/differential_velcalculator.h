#ifndef DIFFERENTIAL_VELCALCULATOR_H
#define DIFFERENTIAL_VELCALCULATOR_H
#include "movement/differential/closedloopcontrol.h"

namespace Movement
{

/*! @brief Wheel velocity calculator for two-wheel (Differential) robots
 * @author Muhaimen Shamsi
 * @details This class exists to provide wheel velocities for the LEGO robots
 * used for code development. It uses the functions defined in closedloopcontrol.h
 * @see ThreeWheelCalculator
 * @see DifferentialCalculator */
class DifferentialCalculator
{
public:
    /*! @brief Velocity calculation with a Point target overload
     * @param goalPoint The target point
     * @see calculateVels */
	wheelvelocities calculateVels
        (Robot* rob, Point goalPoint, float theta_goal, Type moveType);

    /*! @brief Calculate an instantanious threeWheelVels to move torwards a point
     * @param rob Robot to calculate for
     * @param x_goal The X posiiton of the final target
     * @param y_goal the Y position of the final taget
     * @param theta_goal The desired ending facing angle angle
     * @param moveType The Movement Type. See Type
     * @return a wheelvelocities to be sent to the robot's L and R wheels */
	wheelvelocities calculateVels
        (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType);

private:
    ClosedLoopControl    clc;   //!<Calculator instantiation for handling normal movement
    ClosedLoopSharpTurns clst;  //!<Calculator instantiation for handling the SharpTurns type
    ClosedLoopNoSlowdown clnsd; //!<Calculator instantiation for handling the NoSlowdown type
};

}

#endif
