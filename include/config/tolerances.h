#ifndef CONFIG_TOLERANCES_H
#define CONFIG_TOLERANCES_H

#include "math.h"
#include "include/config/simulated.h"
#include "include/config/globals.h"

//! @file
//! @addtogroup config
//! @{

#if SIMULATED
//! The robot's radius in program points.
#define ROBOT_RADIUS 110

/*! How close the robot needs to be to the target in program points
 * before it is considered at that position */
#define DIST_TOLERANCE 50

/*! The max difference in angle for two angles in radians to be
 * considered equal. Test this with Measurements::angleDiff. This
 * is also the default tolerance for the angle version of
 * Measurements::isClose */
#define ROT_TOLERANCE 5.0*M_PI/180.0

/*! An extension beyond the defence area in which we consider a robot
 * to be inside the defence area. Used mostly for path planning */
#define DEF_AREA_TOL ROBOT_RADIUS+10

/*! Variables used to control the speed of a robot dribbling the ball */
#define DRIBBLE_FRWD_SPD 70
#define DRIBBLE_BACK_SPD 7
#define DRIBBLE_TURN_RATE 0.1

#else

#define ROBOT_RADIUS   = 100;
#define DIST_TOLERANCE = 40;
#define ROT_TOLERANCE  = 5.0*(M_PI/180);
#define DEF_AREA_TOL = ROBOT_RADIUS + 10;

#define DRIBBLE_FRWD_SPD = 70;
#define DRIBBLE_BACK_SPD = 7;
#define DRIBBLE_TURN_RATE = 0.1;

#endif
//! @}

#endif




