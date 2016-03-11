#ifndef CONFIG_TOLERANCES_H
#define CONFIG_TOLERANCES_H

//! @file
//! @addtogroup config
//! @{

//! @brief The robot's radius in program points.
extern const int ROBOT_RADIUS;

//! @brief The robot's diameter in program points plus a small overshoot
//! Used for obstacle avoidance calculations
extern const int ROB_OBST_DIA;

/*! @brief How close the robot needs to be to the target in program points
 * before it is considered at that position */
extern const int DIST_TOLERANCE;

/*! @brief The max difference in angle for two angles in radians
 * to be considered equal.
 * @details Test this with Measurements::angleDiff.
 * This is also the default tolerance for the angle version
 * of Measurements::isClose */
extern const double ROT_TOLERANCE;

//! @}

#endif
