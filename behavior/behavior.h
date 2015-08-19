#ifndef BEHAVIOR_H
#define BEHAVIOR_H

class Robot;

/*! @breif Abstract Behavior class which all other behaviors inherit.
 * @author Orign. Narges Ghaedi; JamesW
 * @ingroup Base Classes
 *
 * This is the base class for all behaviors in the robobulls project.
 * it contains a virtual perform(Robot*) function, which is called every
 * loop that a robot has that behavior. The idea is to override this in
 * derived behaviors to perform actions. Also included is a virtual isFinished()
 * function. Child behaviors should override this and return true when the
 * behavior is “finished.”
 */

class Behavior
{
public:
    virtual ~Behavior();
    
    /*! @brief Perform this behavior's action on a robot
     * Given an arbitrary roobot, The <i>perform</i> function
     * of a Behavior defines how to run the behavior on a robot.
     * @param Robot* the robot to perform on. */
    virtual void perform(Robot*) = 0;
    
    /*! @brief Return if behavior is "finished"
     * Return true when the behavior is "finished," false otherwise.
     * This could be reaching a target, or whenever a derived behavior
     * is considered finished. Can be used in Stratigies to effectively
     * assign behaviors one after another.
     * \return True if the behavior is considered finished, false otherwise. */
    virtual bool isFinished();
};


#endif
