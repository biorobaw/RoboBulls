//This is an example showing how to use KickToPointOmni in a Behavior

#include "skill/kicktopointomni.h"

class KickBehavior : public Behavior
{
public:
	//Constructor with a point to kick to
	KickBehavior(Point kickTarget)
	{
		//Initially create the skill the kicking target to a point
		k = new Skill::KickToPointOmni(kickTarget);
	}

	~KickBehavior() 
	{
		delete k;
	}

	void perform(Robot* robot) override
	{
		//Work on performing the kick each iteration
		bool kickFinished = k->perform(robot);

		//KTPO returns true after each state is finished and the  kick is done
		if(kickFinished) {
			std::cout << "KickToPointOmni has finished kicking" << std::endl;
		}
	}

private:
	Skill::KickToPointOmni* k;
}

//This Behavior can be assigned to a robot as follows:
Point kickTarget {400, 0};
robot->assignBeh<KickBehavior>(kickTarget);
