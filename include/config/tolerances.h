#ifndef CONFIG_TOLERANCES_H
#define CONFIG_TOLERANCES_H

//! @file
//! @addtogroup config
//! @{

//! @brief The robot's radius in program points.
extern const int ROBOT_RADIUS;

/*! @brief How close the robot needs to be to the target in program points
 * before it is considered at that position */
extern const int DIST_TOLERANCE;

/*! @brief The max difference in angle for two angles in radians
 * to be considered equal.
 * @details Test this with Measurements::angleDiff.
 * This is also the default tolerance for the angle version
 * of Measurements::isClose */
extern const double ROT_TOLERANCE;


/*! @brief An extension beyond the defence area in which we
 *  consider a robot to be inside the defence area.
 *  Used mostly for path planning */
extern const double DEF_AREA_TOL;

/*! @brief Variables used to control the speed
 *  of a robot dribbling the ball */
extern const double DRIBBLE_FRWD_SPD;
extern const double DRIBBLE_BACK_SPD;
extern const double DRIBBLE_TURN_RATE;

//! @}

#endif
