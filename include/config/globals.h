#ifndef GLOBALS_H
#define GLOBALS_H
//! @file
//! @addtogroup config
//! @{

//! @brief
//! Switch between various field geometries.
//! Only one of these should be set to 1.
#define BIO_ROB_FIELD 0
#define GRSIM_FIELD 0
#define ROBOCUP_FIELD 1

/*! @brief Overall velocity for all behaviors. Change this
 * constant to slow down or speed all robot motion. */
#define OVERALL_VELOCITY 3.0

/*! @brief Approximate ball radius in millimeters.*/
#define BALL_RADIUS 21.5

#if BIO_ROB_FIELD
//! @brief The center-to-goal-line length (X-Axis) of the field
#define HALF_FIELD_LENGTH 3000

//! @brief The full side-to-side length (X-Axis)
#define FIELD_LENGTH  (HALF_FIELD_LENGTH*2)

//! @brief The center-to-side length (Y-Axis) of the field
#define HALF_FIELD_WIDTH 2000

//! @brief The full goal-to-goal length (Y-Axis)
#define FIELD_WIDTH (HALF_FIELD_WIDTH*2)

//! @brief Radius of circular corners of defence area
//! Look at SSL rule book for clarification
#define DEF_AREA_RADIUS 800

//! @brief Distance from Y=0 to the centre of circular areas
//! //! Look at SSL rule book for clarification
#define DEF_AREA_OFFSET 10

//! @brief Width of Goal Post
#define GOAL_WIDTH 1000

#define CENTER_RADIUS 500

#elif GRSIM_FIELD

//! @brief The center-to-goal-line length (X-Axis) of the field
#define HALF_FIELD_LENGTH 3000

//! @brief The full side-to-side length (X-Axis)
#define FIELD_LENGTH  (HALF_FIELD_LENGTH*2)

//! @brief The center-to-side length (Y-Axis) of the field
#define HALF_FIELD_WIDTH 2000

//! @brief The full goal-to-goal length (Y-Axis)
#define FIELD_WIDTH (HALF_FIELD_WIDTH*2)

//! @brief Radius of circular corners of defence area
//! Look at SSL rule book for clarification
#define DEF_AREA_RADIUS 500

//! @brief Distance from Y=0 to the centre of circular areas
//! //! Look at SSL rule book for clarification
#define DEF_AREA_OFFSET 100

//! @brief Width of Goal Post
#define GOAL_WIDTH 1000

//! @brief Conversion factor from meters to field Point coordinates
#define POINTS_PER_METER 1000

//! @brief Conversion factor from points to meters
#define METERS_PER_POINT (1.0 / POINTS_PER_METER)

#define CENTER_RADIUS 500

#elif ROBOCUP_FIELD

//! @brief The center-to-goal-line length (X-Axis) of the field
#define HALF_FIELD_LENGTH 4500

//! @brief The full side-to-side length (X-Axis)
#define FIELD_LENGTH  (HALF_FIELD_LENGTH*2)

//! @brief The center-to-side length (Y-Axis) of the field
#define HALF_FIELD_WIDTH 3000

//! @brief The full goal-to-goal length (Y-Axis)
#define FIELD_WIDTH (HALF_FIELD_WIDTH*2)

//! @brief Radius of circular corners of defence area
//! Look at SSL rule book for clarification
#define DEF_AREA_RADIUS 1000

//! @brief Distance from Y=0 to the centre of circular areas
//! //! Look at SSL rule book for clarification
#define DEF_AREA_OFFSET 250

//! @brief Width of Goal Post
#define GOAL_WIDTH 1000

//! @brief Conversion factor from meters to field Point coordinates
#define POINTS_PER_METER 1000

//! @brief Conversion factor from points to meters
#define METERS_PER_POINT (1.0 / POINTS_PER_METER)

#define CENTER_RADIUS 500

#endif

//! @}

/*! @brief Unused Angle Value for skill/movement constructor code
 * @details This is a constant used to identify, in movement and skill code, an unused
 * angle value. This is needed because in closed loop control and the like,
 * an angle value is required, even though in many cases no angle parameter
 * is desired. */
#define UNUSED_ANGLE_VALUE -10

#endif // GLOBALS_H
