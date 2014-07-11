#ifndef DEFENDCLOSETOBALL_H
#define DEFENDCLOSETOBALL_H

#include "behavior/behavior.h"
#include "utilities/paramlist.h"

class DefendCloseToBall: public Behavior
{
public:
    DefendCloseToBall(const ParameterList& list);
    void perform(Robot *);
};

#endif // DEFENDCLOSETOBALL_H
