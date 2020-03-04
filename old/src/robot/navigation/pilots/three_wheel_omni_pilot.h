//#ifndef THREE_WHEEL_OMNI_PILOT_H
//#define THREE_WHEEL_OMNI_PILOT_H
//#include <cmath>
//#include <vector>
//#include "robot/robot.h"
//#include "movement/movetype.h"
//#include "utilities/measurements.h"
//#include "movement/robot_pilot.h"


///*! @brief Wheel velocity calculator for three-wheel robots
// * @author Muhaimen Shamsi
// * @details This class exists to provide wheel velocities for a three-wheeled robot,
// * where each wheel is approximately 33 degrees apart. Currently, this is used for
// * thre three-wheeled lego goalie Robots at RoboBulls.
// * @see FourWheelCalculator
// * @see DifferentialCalculator */
//class ThreeWheelOmniPilot : public Pilot
//{
//public:
//    /*! @brief Calculate an instantanious threeWheelVels to move torwards a point
//     * @param rob Robot to calculate for
//     * @param x_goal The X posiiton of the final target
//     * @param y_goal the Y position of the final taget
//     * @param theta_goal The desired ending facing angle angle
//     * @param moveType The Movement Type. See Type
//     * @return A threeWheelVels to be sent to the robot's left, right, and back wheels */
//    void drive(Robot* rob, float x_goal, float y_goal, float theta_goal, MoveType moveType);

//    /*! @brief Velocity calculation with a Point target overload
//     * @param goalPoint The target point
//     * @see calculateVels */
//    void drive(Robot* rob, Point goalPoint, float theta_goal, Point nextPoing, MoveType moveType) override;
//private:
//    const double WHEEL_RADIUS = 27;
//    double MAX_MTR_SPD = 100.0;
//    double distance_error, angle_error;

//    //! @brief Movement algorithm to handle Type::Default movement
//    void defaultDrive(Robot* rob, float x_goal, float y_goal, float theta_goal);

//    //! @brief Movement algorithm to handle Type::facePoint movement
//    void facePointDrive(Robot* rob, float x_goal, float y_goal, float theta_goal);

//    //! @brief Approximates bias adjustment values incurred with three-wheel movement
//    std::vector<double> calcBias(double x, double y);
//};



//#endif
