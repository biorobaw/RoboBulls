#ifndef GLOBALS_H
#define GLOBALS_H

#define SIMULATED 1
// Overall velocity for all behaviors
//  10 somewhat fast in the robots
#define OVERALL_VELOCITY 10

/* Defining PI if it is missing */
#ifndef M_PI
 #define M_PI    3.141592653589793
 #define M_PI_2  1.570796326794896
#endif


/* The robot's radius in program points */
#define ROBOT_RADIUS 200


/*
 * DIST_TOLERANCE: How close the robot needs to be to the target
 *    before it is considered at that position
 * DIST_VELOCITY: The forward valocity applied to the motors
 *    when going straight. Keep in mind this is usually not the
 *    actual velocity, as it is constantly rotating.
 */
#define DIST_TOLERANCE 410

#if SIMULATED
 #define DIST_VELOCITY 30
#else
 #define DIST_VELOCITY 80
#endif


/*
 * ROT_TOLERANCE: The range the target and robot angle have to be to each other
 *     to be considered equal (in radians).
 * ROT_VELOCITY:  Velocity applied to each motor in rotation.
 */
#define ROT_TOLERANCE 7*(M_PI/180)

#if SIMULATED
 #define ROT_VELOCITY 5
#else
 #define ROT_VELOCITY 20
#endif


/* FPPA Pathfinding Constants */
#define FPPA_DEBUG 1
#define MAX_RECURSION_DEPTH  3
#define ROBOT_SIZE (ROBOT_RADIUS * 1.75)


#endif // GLOBALS_H
