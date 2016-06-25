#ifndef MOVEMENT_BEHAVIOR_H
#define MOVEMENT_BEHAVIOR_H
#include "behavior/behavior.h"
#include "movement/gotoposition.h"
#include "movement/movetype.h"

//! @ingroup everydayuse baseclasses
//! @{

/*! @brief Behavior to perform generic point-to-point Movement
 * @author JamesW
 * @details This class interfaces with a Movement::GoToPosition object to provide
 * Behaviors a simple method of moving from point to point. If a Behavior's only function
 * is to perform movement with no other actions, GenericMovementBehavior is a good baseclass.
 *
 * <b>Example Everyday Usage</b>
 * @include example_genericmovement.cpp */

class GenericMovementBehavior : public Behavior
{
public:
    /*! @brief Equivalent to calling setMovementTargets initially */
    GenericMovementBehavior(
        Point target    = Point(0,0),
        float angTarget = UNUSED_ANGLE_VALUE,
        bool  obsAvoid  = true,
        bool  ballAvoid = true);

   //! @brief Performs the movement after setMovementTargets is set */
   void perform(Robot* robot) override;
   
   //! @brief Return true when reached the movement targets
   bool isFinished() override;

protected:

    /*! @brief Perform movement to move to movement targets
     * @details use GenericMovementBehavior::perform(robot); */
    void perform(Robot* robot, Movement::Type type);

    //! @{
    /*! @brief Sets the next movement targets (Point target and angle) to move to
     * @details This is the most important function; after a derived class has calculated the points
     * to move to, use setMovementTargets to recreate the GoToPosition object to those
     * parameters: point, angle, and use obstacle avoidance or not */
    void setMovementTargets(Point targetPoint, float targetAngle = UNUSED_ANGLE_VALUE);
    void setMovementTargets(Point targetPoint, float targetAngle,  bool useObstacleAvoid, bool useAvoidBall = true);
    //! @}
        
    //! @brief Set the velocity multiplier on the Movement object.
    void setVelocityMultiplier(float newMultiplier);

    //! @brief Set the movement tolerances on the Movement object.
    void setMovementTolerances(float newDistolerance, float newRotTolerance);
    
private:
    Movement::GoToPosition movement;
    float velocityMultipier;
    bool  useObstacleAvoid;
    bool  useAvoidBall;
    bool  movementFinished;
};

#endif
