#ifndef CONFIG_TOLERANCES_H
#define CONFIG_TOLERANCES_H

//! @file Defines default tolerances and constants for movement and rotation

//! @brief The robot's radius in program points.
extern const int ROBOT_RADIUS;

//! @brief The robot's diameter in program points plus a small overshoot
extern const int ROBOT_SIZE;

/*! @brief How close the robot needs to be to the target in program points
 * before it is considered at that position */
extern const int DIST_TOLERANCE;

/*! @brief The max difference in angle for two angles in radians
 * to be considered equal.
 * @details Test this with Measurements::angleDiff.
 * This is also the default tolerance for the angle version
 * of Measurements::isClose */
extern const int ROT_TOLERANCE;

#endif
