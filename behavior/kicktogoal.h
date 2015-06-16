#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/point.h"

/* Since this is only used in FreeKickStrategy, there is a hack for the robot
 * to not kick unless all robots are stopped. Don't use this. Just use normal KTPO:
 * ex: robot->assignSkill<KickToPointOmni>(gameModel->getOpponentGoal());
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
