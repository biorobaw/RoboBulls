#ifndef BEHAVIOR_H
#define BEHAVIOR_H

//#include "model/robot.h"
//#include "model/gamemodel.h"

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
