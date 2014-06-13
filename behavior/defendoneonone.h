#ifndef DEFENDONEONONE_H
#define DEFENDONEONONE_H

#include "behavior.h"
#include "utilities/paramlist.h"

class Robot;

class DefendOneOnOne : public Behavior
{
public:
    DefendOneOnOne(const ParameterList& list);

    void perform(Robot * r);
};

#endif // DEFENDONEONONE_H
