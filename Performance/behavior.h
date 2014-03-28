#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Robot/robot.h"
#include "Robot/ball.h"
class Robot;
class Behavior
{
public:
    Behavior();
    virtual void perform(Robot *, Ball);
};

#endif // BEHAVIOR_H
