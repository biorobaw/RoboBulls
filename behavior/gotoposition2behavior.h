#ifndef GOTOPOSITION2BEHAVIOR_H
#define GOTOPOSITION2BEHAVIOR_H

#include "utilities/point.h"
#include "behavior.h"
#include "skill/gotoposition2.h"
#include "utilities/paramlist.h"

class GoToPosition2Behavior : public Behavior
{
public:
    GoToPosition2Behavior(const ParameterList& list);
    void perform(Robot* robot);

private:
	enum {rotating, moving, idling} state;
    Point mTargetPoint;
	Skill::GoToPosition2* theSkill;
};

#endif // GOTOPOSITION2BEHAVIOR_H
