#ifndef DEFENDCLOSETOBALL_H
#define DEFENDCLOSETOBALL_H

#include "behavior/genericmovementbehavior.h"

class DefendCloseToBall: public GenericMovementBehavior
{
public:
	DefendCloseToBall(const ParameterList& list);
    void perform(Robot*) override;
};

#endif // DEFENDCLOSETOBALL_H
