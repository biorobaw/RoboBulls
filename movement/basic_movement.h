#ifndef BASIC_MOVEMENT_H
#define BASIC_MOVEMENT_H

#include "model/robot.h"
#include "movement/move.h"

namespace Movement
{

class GoForward
{
public:
    void perform(Robot* rob);
};


class GoBackward
{
public:
     void perform(Robot* rob);
};

}


#endif

