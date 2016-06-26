#ifndef MARKBOT_H
#define MARKBOT_H

#include "genericmovementbehavior.h"
#include "model/gamemodel.h"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"
#include "gui/guiinterface.h"
#include "skill/kicktopointomni.h"

class MarkBot : public GenericMovementBehavior
{
public:
    MarkBot();
    void perform(Robot *) override;
    bool isFinished() override;

private:
    // Variables for marking opponents
    static bool mark_status[10];
    int marked_opp_id = -1;
    void updateMark();
};

#endif // MARKBOT_H
