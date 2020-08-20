#ifndef MOVEMENT_BEHAVIOR_H
#define MOVEMENT_BEHAVIOR_H
#include "model/robot/behavior.h"
#include "utilities/point.h"
#include "model/robot/navigation/commands/CmdGoToPose.h"


//! @ingroup everydayuse baseclasses
//! @{

/*! @file
 * Behavior to perform generic point-to-point Movement
 * This class interfaces with a Movement::GoToPose object to provide
 * Behaviors a simple method of moving from point to point. If a Behavior's only function
 * is to perform movement with no other actions, GenericMovementBehavior is a good baseclass.
 *
 * <b>Example Everyday Usage</b>
 * @include example_genericmovement.cpp */

class GenericMovementBehavior : virtual public Behavior
{
public:
    GenericMovementBehavior(Robot* robot,
                            Point targetPose=Point(0,0),
                            float targetAngle = UNUSED_ANGLE_VALUE,
                            bool avoidObstacles = true,
                            bool avoidBall = false);

   //! @brief Performs the movement after setMovementTargets is set */
   bool perform() override;
   
   //! @brief Return true when reached the movement targets
   bool isFinished() override;

   virtual string getName() override;
    
protected:
    CmdGoToPose cmd;
    bool done;
};

#endif
