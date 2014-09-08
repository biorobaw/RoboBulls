#ifndef DEFENDONEONONE_H
#define DEFENDONEONONE_H

#include "behavior/genericmovementbehavior.h"
#include "model/robot.h"

class DefendOneOnOne : public GenericMovementBehavior
{
public:
    DefendOneOnOne(const ParameterList& list);

    void perform(Robot * r);
};

#endif // DEFENDONEONONE_H
