#ifndef GLOBALS_H
#define GLOBALS_H

//! @file Contains global constans such as field widths and lengths

/*! @brief Overall velocity for all behaviors Change this
 * constant to slow down or speed up the whole game. */
#define OVERALL_VELOCITY 1

//! @brief Defining PI if it is missing
#ifndef M_PI
 #define M_PI    3.141592653589793
 #define M_PI_2  1.570796326794896
#endif

//! @briefThe "long ways" i.e, center-to-goal length of the standard RoboCup field
#define FIELD_LENGTH   3000
#define FIELD_2LENGTH  (FIELD_LENGTH*2)

//! @brief The "short-ways" i,e, center-to-side length of the standard RoboCup field
#define FIELD_WIDTH     2000
#define FIELD_2WIDTH   (FIELD_WIDTH*2)

//! @brief X length (long ways) width of goalie box
#define GOAL_LENGTH     500

//! @brief Y length (short-ways) length of goalie box
#define GOAL_WIDTH      700

/*! @brief Use this on unused parameters to a function to disable
 * "unused parameter" warnings. */
#define UNUSED_PARAM(x) (void)(x)

/*! @brief Unused Angle Value for skill/movement constructor code
 * @details This is a constant used to identify, in movement and skill code, an unused
 * angle value. This is needed because in closed loop control and the like,
 * and angle value is required, even though in many cases no angle parameter
 * is desired. */
#define UNUSED_ANGLE_VALUE -10


#endif // GLOBALS_H
