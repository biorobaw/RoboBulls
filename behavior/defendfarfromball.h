#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H

#include "behavior/genericmovementbehavior.h"

class DefendFarFromBall : public GenericMovementBehavior
{
public:
    DefendFarFromBall(const ParameterList& list);
    void perform(Robot*);
};

#endif // DEFENDFARFROMBALL_H
