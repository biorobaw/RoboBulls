#ifndef SIMPLEBEHAVIORS_H
#define SIMPLEBEHAVIORS_H

#include "behavior/behavior.h"

/* SimpleBehaviors
 * SimpleBehaviors is a class to handle three gamestates
 * that would be inefficient to make their own files.
 * This is used for robots that have no role in these behaviors.
 * 'F' 'f' : Freekicks
 * 'T' 't' : Timeouts
 * 'H' 'h' : Halt
 * 'P' 'p' : Penalty kicks
 */

class SimpleBehaviors : public Behavior
{
public:
    void perform(Robot*);
};

#endif // SIMPLEBEHAVIORS_H
