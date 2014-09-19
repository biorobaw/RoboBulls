#ifndef MOVE_TYPE_H
#define MOVE_TYPE_H

namespace Movement
{

/* The purpose of this file is for movement *modifiers*; for instance,
 * differential robots have sharp turns, no slowdown, and others; the omni
 * robots have max velocity and speed versions of their default movement 
 * calculators. These enums recognize all modifier types, and are passed to 
 * GoToPosition. In the case that the modifier is not supported by the 
 * robot type GoToPosition has been passed, the default movement calculation
 * is used.
 */
typedef char MovementType;


enum class Type : MovementType
{
    Default = -1,

    /*** Differential Movement Types ***/
    SharpTurns,
	NoSlowdown,
    Count,			//Always leave last; do not use

    /*** Omni Movement Types ***/
    facePoint
};


#define IS_DIFFERENTIAL(type) \
    ((MovementType)(type) >= 0 && (MovementType)(type) < (MovementType)Type::Count)
	
#define IS_OMNI(type) \
    ((MovementType)(type) > (MovementType)Type::Count)
	
}

#endif
