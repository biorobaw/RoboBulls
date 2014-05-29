#ifndef GOTOPOSITION2_H
#define GOTOPOSITION2_H

#include "skill/skill.h"
#include "model/robot.h"

namespace Skill
{

class GoToPosition2 : public Skill
{
public:
	GoToPosition2(float x, float y);
	GoToPosition2(Point target);
	void perform(Robot *robot);
private:
	Point mTargetPoint;
	enum states {rotating, moving, idling} state;
};

}

#endif // GOTOPOSITION2_H
