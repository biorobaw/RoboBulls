#ifndef GLOBALS_H
#define GLOBALS_H

// SIMULATED 1 ==> running simulator
//SIMULATED 0 ==> running robots on the field
#define SIMULATED 1
// Overall velocity for all behaviors
//  10 somewhat fast in the robots
#define OVERALL_VELOCITY 10

/**
  The player's team. 0 is BLUE, 1 is Yellow.
  */
#define TEAM 0


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
#define DIST_VELOCITY 80



/*
 * ROT_TOLERANCE: The range the target and robot angle have to be to each other
 *     to be considered equal (in radians).
 * ROT_VELOCITY:  Velocity applied to each motor in rotation.
 */
#define ROT_TOLERANCE 7*(M_PI/180)
#define ROT_VELOCITY 20



/* FPPA Pathfinding Constants */
#define FPPA_DEBUG 0
#define MAX_RECURSION_DEPTH  3
#define ROBOT_SIZE (ROBOT_RADIUS * 2.5)


#endif // GLOBALS_H
