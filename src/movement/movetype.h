#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H

/*! @brief Unused Angle Value for skill/movement constructor code
 * @details This is a constant used to identify, in movement and skill code, an unused
 * angle value. This is needed because in closed loop control and the like,
 * an angle value is required, even though in many cases no angle parameter
 * is desired. */
#define UNUSED_ANGLE_VALUE -10



namespace Move
{

//! @file Movement Type modifiers used in calculations and perform() parameters

/*! @brief Defines Movement Type modifiers
 * @author JamesW
 *
 * The purpose of this enum is for movement <i>modifiers</i>; for instance,
 * differential robots have sharp turns, no slowdown, and others; the omni
 * robots have max velocity and speed versions of their default movement 
 * calculators. In the case that the modifier is not supported by the
 * robot type go_to_pose has been passed, the default movement calculation
 * is used.
 */
enum MoveType
{
    Default = -1,

    // Differential Movement Types
    SharpTurns, //Try to make more shaper curves around object
    NoSlowdown, //Attempts to not have as much of a slowdown when reaching a point
    DiffCount,  //Leave last and do not use

    // Omni Movement Types
    facePoint,  //Puts more emphasis on rotation
    dribble    //Used when dribbling the ball
};


//!< @brief Determines if a given Movement::Type is Differenetial
#define IS_DIFFERENTIAL(type) (((type) >= 0) && ((type) < MoveType::DiffCount))

//!< @brief Determines if a given Movement::Type is Omni
#define IS_OMNI(type) ((type) > MoveType::DiffCount)

}

#endif
