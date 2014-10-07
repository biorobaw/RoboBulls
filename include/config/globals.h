﻿#ifndef GLOBALS_H
#define GLOBALS_H

/* Overall velocity for all behaviors
 * Change this constant to slow down or speed up
 * the whole game.
 */
#define OVERALL_VELOCITY 1

/* Defining PI if it is missing */
#ifndef M_PI
 #define M_PI    3.141592653589793
 #define M_PI_2  1.570796326794896
#endif


/* The "long ways" i.e, goal-to-goal length of the standard
 * RoboCup field
 */
#define FIELD_LENGTH   3000
#define FIELD_LENGTH_2 (FIELD_LENGTH/2)

/* The "short-ways" i,e, side-to-side length of the standard
 * RoboCup field
 */
#define FIELD_WIDTH   1500
#define FIELD_WIDTH_2 (FIELD_WIDTH/2)



/* Use this on unused parameters to disables "unused parameter" warnings.
   e.g, in Behavior constructors with the ParameterList params */
#define UNUSED_PARAM(x) (void)(x)

/* This is a constant used to identify, in movement and skill code, an unused
 * angle value. This is needed because in closed loop control and the like,
 * and angle value is required, even though in many cases no angle parameter
 * is desired
 */
#define UNUSED_ANGLE_VALUE -10


#endif // GLOBALS_H
