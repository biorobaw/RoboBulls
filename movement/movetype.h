#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H

namespace Movement
{

//! @file

/*! @brief Defines Movement Type modifiers
 * @author JamesW
 *
 * The purpose of this file is for movement <i>modifiers</i>; for instance,
 * differential robots have sharp turns, no slowdown, and others; the omni
 * robots have max velocity and speed versions of their default movement 
 * calculators. These enums recognize all modifier types, and are passed to 
 * GoToPosition. In the case that the modifier is not supported by the 
 * robot type GoToPosition has been passed, the default movement calculation
 * is used.
 */
enum Type
{
    Default = -1,

    //Differential Movement Types
    SharpTurns,
	NoSlowdown,
    DiffCount,  //Leave last and do not use

    //Omni Movement Types
    facePoint
};


//!< @brief Determines if a given Movement::Type is Differenetial
#define IS_DIFFERENTIAL(type) (((type) >= 0) && ((type) < Type::DiffCount))

//!< @brief Determines if a given Movement::Type is Omni
#define IS_OMNI(type) ((type) > Type::DiffCount)
	
}

#endif
