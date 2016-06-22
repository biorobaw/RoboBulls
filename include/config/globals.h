#ifndef GLOBALS_H
#define GLOBALS_H
//! @file
//! @addtogroup config
//! @{

//! @brief
//! Use the dimensions of the field in the bio-robotics lab
//! as opposed to standard SSL field dimensions
#define BIO_ROB_FIELD 1

/*! @brief Overall velocity for all behaviors. Change this
 * constant to slow down or speed up the whole game. */
#define OVERALL_VELOCITY 1.5

//! @brief The center-to-goal length (X-Axis) of the standard RoboCup field
#define HALF_FIELD_LENGTH 3000

//! @brief The full side-to-side length (X-Axis)
#define FIELD_LENGTH  (HALF_FIELD_LENGTH*2)

//! @brief The center-to-side length (Y-Axis) of the standard RoboCup field
#define HALF_FIELD_WIDTH 2000

//! @brief The full goal-to-goal length (Y-Axis)
#define FIELD_WIDTH (HALF_FIELD_WIDTH*2)

//! @brief Radius of circular corners of defence area
//! Look at SSL rule book for clarification
#if BIO_ROB_FIELD
#define DEF_AREA_RADIUS 800
#else
#define DEF_AREA_RADIUS 500
#endif

//! @brief Distance from Y=0 to the centre of circular areas
//! //! Look at SSL rule book for clarification
#if BIO_ROB_FIELD
#define DEF_AREA_OFFSET 10
#else
#define DEF_AREA_OFFSET 250
#endif

//! @brief Width of Goal Post
#define GOAL_WIDTH 1000

//! @brief Conversion factor from meters to field Point coordinates
#define POINTS_PER_METER 1250

//! @brief Conversion factor from Points to meters
#define METERS_PER_POINT (1.0 / POINTS_PER_METER)

#define BALL_RADIUS 23

//! @}

/*! @brief Unused Angle Value for skill/movement constructor code
 * @details This is a constant used to identify, in movement and skill code, an unused
 * angle value. This is needed because in closed loop control and the like,
 * an angle value is required, even though in many cases no angle parameter
 * is desired. */
#define UNUSED_ANGLE_VALUE -10

#endif // GLOBALS_H
