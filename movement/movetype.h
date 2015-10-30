#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H

namespace Movement
{

//! @file Movement Type modifiers used in calculations and perform() parameters

/*! @brief Defines Movement Type modifiers
 * @author JamesW
 *
 * The purpose of this enum is for movement <i>modifiers</i>; for instance,
 * differential robots have sharp turns, no slowdown, and others; the omni
 * robots have max velocity and speed versions of their default movement 
 * calculators. In the case that the modifier is not supported by the
 * robot type GoToPosition has been passed, the default movement calculation
 * is used.
 */
enum Type
{
    Default = -1,

    //Differential Movement Types
    SharpTurns, //Try to make more shaper curves around object
    NoSlowdown, //Attempts to not have as much of a slowdown when reaching a point
    DiffCount,  //Leave last and do not use

    //Omni Movement Types
    facePoint,  //Puts more emphasis on rotation
    StayStill   //Used when the robot shoudl stay in it's current spot
};


//!< @brief Determines if a given Movement::Type is Differenetial
#define IS_DIFFERENTIAL(type) (((type) >= 0) && ((type) < Type::DiffCount))

//!< @brief Determines if a given Movement::Type is Omni
#define IS_OMNI(type) ((type) > Type::DiffCount)
	
}

#endif
