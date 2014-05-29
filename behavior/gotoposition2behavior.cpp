#include "behavior/gotoposition2behavior.h"
#include "skill/gotoposition2.h"
#include "model/gamemodel.h"

GoToPosition2Behavior::GoToPosition2Behavior(const ParameterList& list)
{
	this->mTargetPoint = list.getParam<Point>("targetPoint");
    theSkill = new Skill::GoToPosition2(mTargetPoint);

    std::cout << "GP2: Target: " << mTargetPoint.toString() << std::endl;
}

void GoToPosition2Behavior::perform(Robot * robot)
{
    if(theSkill != NULL) {
        theSkill->perform(robot);
    }
    if(Measurments::isClose(robot->getRobotPosition(), mTargetPoint, 1.05*DIST_TOLERANCE)
            && theSkill != NULL) {
        std::cout << "Deleting Skill" << std::endl;
        delete theSkill;
        theSkill = NULL;
    }
}
