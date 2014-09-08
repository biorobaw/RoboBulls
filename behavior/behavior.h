#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "utilities/paramlist.h"

/* UNUSED_PARAM is mostly used in Behaviors, so define it here
 * to avoid having to include globals.h.
 */
#ifndef UNUSED_PARAM
 #define UNUSED_PARAM(x) (void)(x)
#endif

class Robot;

/*
 * General Behavior class which inheritas other behaviors
 * Narges Ghaedi
 */

class Behavior
{
public:
    Behavior();
    virtual void perform(Robot*) = 0;
    virtual ~Behavior(){}
};


#endif // BEHAVIOR_H
