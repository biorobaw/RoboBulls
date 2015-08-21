#ifndef MOVEMENT_BEHAVIOR_H
#define MOVEMENT_BEHAVIOR_H
#include "behavior/behavior.h"
#include "movement/gotoposition.h"
#include "movement/movetype.h"

/*! @brief A Behavior-level interface to Movement
 * @author JamesW
 * @details This purpose of this Behavior is to provide an interface to the
 * Movement layer of the "velocity sending" hierarchy. The reason is that many
 * behaviors directly used deprecated things like CLC, and also because there exists
 * a lot of boilerplate in managing a pointer to a GoToPosition object. This class
 * simply manages internally a GoToPositon object and provides an interface to it.
 * The basic usage idea--in a perform() function of a derived class--
 * is to call setMovementTargets, and then call GenericMovementBehavior::perform. */

class GenericMovementBehavior : public Behavior
{
public:
    /*! @brief Constructor is also an interface to a GoToPosition object.
     * @details It can set up the movement object on construction
     * instead of being defaulted to 0,0
     * @see GoToPosition
     */    
    GenericMovementBehavior(
        Point target    = Point(0,0), 
        float angTarget = UNUSED_ANGLE_VALUE, 
        bool  obsAvoid  = true, 
        bool  ballAvoid = true);
        
   ~GenericMovementBehavior();
   
   void perform(Robot* robot) override;
   
   /* isFinished override
    * return true when the movement object has reached the movement
    * targets
    */
   bool isFinished() override;

protected:

    /*! @brief Calls the GoToPosition object
     * @details This actually calculates and sets the velocities on the robot via .
     * GoToPosition. After a derived class has obtained a point and angle 
     * (and set them with setMovementTargets), call this to calculate wheelvels 
     * and set them on the robot; use  GenericMovementBehavior::perform(robot); */
    void perform(Robot* robot, Movement::Type type);

    //! @{
    /*! @brief Sets the next movement targets (Point target and angle) to move to
     * @details This is the most important function; after a derived class has calculated the points
     * to move to, use setMovementTargets to recreate the GoToPosition object to those
     * parameters: point, angle, and use obstacle avoidance or not */
    void setMovementTargets(Point targetPoint, float targetAngle = UNUSED_ANGLE_VALUE);
    void setMovementTargets(Point targetPoint, float targetAngle,
                            bool useObstacleAvoid, bool useAvoidBall = true);
    //! @}
        
    //! @brief Set the velocity multiplier on the Movement object.
    void setVelocityMultiplier(float newMultiplier);

    //! @brief Set the movement tolerances on the Movement object.
    void setMovementTolerances(float newDistolerance, float newRotTolerance);
    
private:
    Movement::GoToPosition* movement;
    float velocityMultipier;
    float newDistTolerance;
    float newRotTolerance;
    bool  useObstacleAvoid;
    bool  useAvoidBall;
    bool  movementFinished;
};

#endif
