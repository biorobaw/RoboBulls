#ifndef BEHAVIOR_H
#define BEHAVIOR_H

/* UNUSED_PARAM is mostly used in Behaviors, so define it here
 * to avoid having to include globals.h.
 */
#ifndef UNUSED_PARAM
 #define UNUSED_PARAM(x) (void)(x)
#endif

class Robot;

/*
 * General Behavior class which inheritas other behaviors
 * Narges Ghaedi et al
 */
class Behavior
{
public:
    virtual ~Behavior(){}
    virtual void perform(Robot*) = 0;
};


#endif // BEHAVIOR_H
