#ifndef KICKTOGOAL_H
#define KICKTOGOAL_H

#include "behavior/behavior.h"
#include "skill/stop.h"
#include "utilities/point.h"

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
    Point target;
    bool sign;  //0 is positive and 1 is negative
    bool targetSign;
    Skill::Skill* kickToPoint = nullptr;
    Point ballOrig;
};

#endif // KICKTOGOAL_H
