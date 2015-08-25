#ifndef SIMPLEBEHAVIORS_H
#define SIMPLEBEHAVIORS_H

#include "behavior/behavior.h"

/*! @brief Behavior to handle simple Strategy cases
 * @details SimpleBehaviors is a class to handle three gamestates
 * that would be inefficient to make their own files.
 * This is used for robots that have no role in these behaviors.<br>
 * 'F' 'f' : Freekicks<br>
 * 'T' 't' : Timeouts<br>
 * 'H' 'h' : Halt<br>
 * 'P' 'p' : Penalty kicks
 */

class SimpleBehaviors : public Behavior
{
public:
    void perform(Robot*);
};

#endif // SIMPLEBEHAVIORS_H
