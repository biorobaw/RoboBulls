#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H

#include "behavior/behavior.h"
#include "utilities/paramlist.h"

class DefendFarFromBall : public Behavior
{
public:
    DefendFarFromBall(const ParameterList& list);
    void perform(Robot*);
};

#endif // DEFENDFARFROMBALL_H
