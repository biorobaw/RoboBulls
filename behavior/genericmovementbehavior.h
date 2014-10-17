#ifndef MOVEMENT_BEHAVIOR_H
#define MOVEMENT_BEHAVIOR_H

#include "behavior/behavior.h"
#include "movement/gotoposition.h"
#include "movement/movetype.h"
#include "utilities/paramlist.h"

/* GenericMovementBehavior
 * This purpose of this Behavior is to provide an interface to the 
 * Movement layer of the "velocity sending" hierarchy. The reason is that many
 * behaviors directly used deprecated things like CLC, and also because there exists
 * a lot of boilerplate in managing a pointer to a GoToPosition object. This class
 * simply manages internally a GoToPositon object and provides an interface to it.
 * The basic usage idea--in a perform() function of a derived class--
 * is to call setMovementTargets, and then call GenericMovementBehavior::perform.
 */

class GenericMovementBehavior : public Behavior
{
public:
    /* This ParameterList constructor is also an interface to constructing the
     * GoToPosition object. It can take three parameters to construct the GoToPosition,
     * on construction of the behavior itself.
     * - "targetPoint" : (Point) : target point
     * - "targetAngle" : (float) : target angle
     * - "obstacleAvoidance" : (bool) : Use obstacle avoidance?
     * Keep in mind this is an alternative to setMovementTargets; it allows the 
     * movement target parameters to be set on construction.
     */
    GenericMovementBehavior();
    GenericMovementBehavior(const ParameterList& list);
   ~GenericMovementBehavior();

protected:

    /* This actually calculates and sets the velocities on the robot via . 
     * GoToPosition. After a derived class has obtained a point and angle 
     * (and set them with setMovementTargets), call this to calculate wheelvels 
     * and set them on the robot; use  GenericMovementBehavior::perform(robot);
     */
    void perform(Robot* robot, Movement::Type type = Movement::Type::Default);

    /* This is the most important function; after a derived class has calculated the points
     * to move to, use setMovementTargets to recreate the GoToPosition object to those
     * parameters: point, angle, and use obstacle avoidance or not.
     */
    void setMovementTargets(Point targetPoint, float targetAngle = UNUSED_ANGLE_VALUE);
    void setMovementTargets(Point targetPoint, float targetAngle, bool useObstacleAvoid);
        
    /* Set the velocity multiplier on the Movement object.
     * Like old times, except now 1.0 is the normal multiplier, and 0 means no velocity.
     */
    void setVelocityMultiplier(float newMultiplier);
    void setMovementTolerances(float newDistolerance, float newRotTolerance);
    
private:
    Movement::GoToPosition* movement;
    float velocityMultipier;
    float newDistTolerance;
    float newRotTolerance;
    bool  useObstacleAvoid;
};

#endif
