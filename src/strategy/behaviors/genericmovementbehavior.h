#ifndef MOVEMENT_BEHAVIOR_H
#define MOVEMENT_BEHAVIOR_H
#include "../behavior.h"
#include "utilities/point.h"
#include "robot/navigation/commands/CmdGoToPose.h"


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

class GenericMovementBehavior : public Behavior
{
public:
    GenericMovementBehavior(Point targetPose=Point(0,0),
                            float targetAngle = UNUSED_ANGLE_VALUE,
                            bool avoidObstacles = true,
                            bool avoidBall = false);

   //! @brief Performs the movement after setMovementTargets is set */
   void perform(Robot* robot) override;
   
   //! @brief Return true when reached the movement targets
   bool isFinished() override;
    
protected:
    CmdGoToPose cmd;
    bool done;
};

#endif
