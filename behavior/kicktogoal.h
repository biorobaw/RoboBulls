#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/point.h"

/*! @brief Old Behavior to kick to the opponent's goal
 * @details <b>Don't use. </b> Behavior used in FreeKickStrategy to kick to goal
 * Contains a hack that only work in that Strategy
 * @deprecated Use robot->assignSkill<KickToPointOmni>(gameModel->getOpponentGoal());
 */
class KickToGoal : public Behavior
{
public:
    KickToGoal();
    ~KickToGoal();
    void perform(Robot * r);
    bool isFinished() override;
private:
    enum StateEnum {initial, kicking, stopping};
    StateEnum state;
    Skill::Skill* kickToPoint = nullptr;
    Point ballOrig;

    //Ensures kicker does not move until robots are stopped
    int  delayCount;
    bool checkifOkay();
    bool okay;
};

#endif // KICKTOGOAL_H
