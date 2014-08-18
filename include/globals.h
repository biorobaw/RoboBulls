#ifndef GLOBALS_H
#define GLOBALS_H

// SIMULATED 1 ==> running simulator
//SIMULATED 0 ==> running robots on the field
#define SIMULATED 1
// Overall velocity for all behaviors
//  10 somewhat fast in the robots
#define OVERALL_VELOCITY 5

/**
  The player's team. 0 is BLUE, 1 is Yellow.
  */
#define TEAM 0

#if SIMULATED
#define CLC_ROTATONG_VEL 5
#else
#define CLC_ROTATONG_VEL 1
#endif

/* Defining PI if it is missing */
#ifndef M_PI
 #define M_PI    3.141592653589793
 #define M_PI_2  1.570796326794896
#endif


/* The robot's radius in program points */
#if SIMULATED
 #define ROBOT_RADIUS 100
#else
 #define ROBOT_RADIUS 200
#endif

/*
 * DIST_TOLERANCE: How close the robot needs to be to the target
 *    before it is considered at that position
 * DIST_VELOCITY: The forward velocity applied to the motors
 *    when going straight. Keep in mind this is usually not the
 *    actual velocity, as it is constantly rotating.
 */
#if SIMULATED
    #define DIST_TOLERANCE 10
    #define DIST_VELOCITY 80
#else
    #define DIST_TOLERANCE 100
    #define DIST_VELOCITY 80
#endif


/*
 * ROT_TOLERANCE: The max difference in angle for two angles to be considered equal
 * ROT_VELOCITY:  Velocity applied to each motor in rotation.
 */
#if SIMULATED
    #define ROT_TOLERANCE 5*(M_PI/180)
    #define ROT_VELOCITY 20
#else
    #define ROT_TOLERANCE 10*(M_PI/180)
    #define ROT_VELOCITY 20
#endif



/* FPPA Pathfinding Constants */
#define FPPA_DEBUG 0
#define MAX_RECURSION_DEPTH  3
#define ROBOT_SIZE (ROBOT_RADIUS * 2.5)

/* Prints out which behaviors and skills are running */
#define TRACE 1

#endif // GLOBALS_H
