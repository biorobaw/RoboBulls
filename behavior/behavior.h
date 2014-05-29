#ifndef BEHAVIOR_H
#define BEHAVIOR_H

//#include "model/robot.h"
//#include "model/gamemodel.h"

class Robot;

class Behavior
{
public:
    Behavior();
    virtual void perform(Robot*) = 0;
};


#endif // BEHAVIOR_H
