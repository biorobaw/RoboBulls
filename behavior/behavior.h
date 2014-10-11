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
    enum class Priority : int {
        None = -1,
        VeryLow,
        Low,
        Medium,
        High,
        VeryHigh
    };

    Behavior(std::string name = "Behavior", Priority pri = Priority::Medium);
    virtual ~Behavior(){}
    virtual void perform(Robot*) = 0;
    int getPriority();
    std::string toString();

private:
    Priority priority;
    std::string name;
};


#endif // BEHAVIOR_H
