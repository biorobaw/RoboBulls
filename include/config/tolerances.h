#ifndef CONFIG_TOLERANCES_H
#define CONFIG_TOLERANCES_H

#include "include/config/simulated.h"

/* The robot's radius in program points */
#if SIMULATED
 #define ROBOT_RADIUS 100
#else
 #define ROBOT_RADIUS 200
#endif

/* DIST_TOLERANCE: How close the robot needs to be to the target
 * before it is considered at that position
 */
#if SIMULATED
 #define DIST_TOLERANCE 30
#else
 #define DIST_TOLERANCE 100
#endif


/* ROT_TOLERANCE: The max difference in angle for two angles to be 
 * considered equal. Test this with Measurements::angleDiff
 * This is also the default tolerance for the angle version
 * of Measurements::isClose
 */
#if SIMULATED
 #define ROT_TOLERANCE 5*(M_PI/180)
#else
 #define ROT_TOLERANCE 10*(M_PI/180)
#endif

#endif
