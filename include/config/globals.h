#ifndef GLOBALS_H
#define GLOBALS_H
//! @file
//! @addtogroup config
//! @{

/*! @brief Overall velocity for all behaviors. Change this
 * constant to slow down or speed up the whole game. */
#define OVERALL_VELOCITY 1

//! @brief The center-to-goal length (X-Axis) of the standard RoboCup field
#define FIELD_LENGTH   3000

//! @brief The full side-to-side length (X-Axis)
#define FIELD_2LENGTH  (FIELD_LENGTH*2)

//! @brief The center-to-side length (Y-Axis) of the standard RoboCup field
#define FIELD_WIDTH     2000

//! @brief The full goal-to-goal length (Y-Axis)
#define FIELD_2WIDTH   (FIELD_WIDTH*2)

//! @brief X length (long ways) width of goalie box
#define GOAL_LENGTH     500

//! @brief Y length (short-ways) length of goalie box
#define GOAL_WIDTH      700

//! @}

/*! @brief Unused Angle Value for skill/movement constructor code
 * @details This is a constant used to identify, in movement and skill code, an unused
 * angle value. This is needed because in closed loop control and the like,
 * and angle value is required, even though in many cases no angle parameter
 * is desired. */
#define UNUSED_ANGLE_VALUE -10

#endif // GLOBALS_H
