#ifndef REFSTOP_H
#define REFSTOP_H

#include "behavior/genericmovementbehavior.h"
#include "utilities/comparisons.h"
#include "utilities/region/defencearea.h"

class RefStop : public GenericMovementBehavior
{
public:
    RefStop();
    void perform(Robot*) override;
    bool isFinished();
private:
    bool finished;
};

#endif // REFSTOP_H
