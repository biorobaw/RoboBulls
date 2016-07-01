#ifndef PENALTYGOALIE_H
#define PENALTYGOALIE_H

#include "behavior/genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "model/gamemodel.h"
#include "gui/guiinterface.h"


class PenaltyGoalie : public GenericMovementBehavior
{
public:
    PenaltyGoalie();
    void perform(Robot*) override;
    bool isFinished() override;
private:

};

#endif // PENALTYGOALIE_H
