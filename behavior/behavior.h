#ifndef BEHAVIOR_H
#define BEHAVIOR_H

class Robot;

/*
 * General Behavior class which inheritas other behaviors
 * Narges Ghaedi ``et al``
 */
class Behavior
{
public:
    virtual ~Behavior();
    
    /* `perform`
     * Mandatory. Called each loop while assigned to a robot; 
     * Perform this behavior's action on an arbitrary robot.
     */
    virtual void perform(Robot*) = 0;
    
    /* `isFinished`
     * Return true when the behavior is "finished," false otherwise.
     * This could be reaching a target, or whenever a derived behavior
     * is considered finished. Can be used in Stratigies to effectively
     * assign behaviors one after another.
     */
    virtual bool isFinished();
};


#endif
