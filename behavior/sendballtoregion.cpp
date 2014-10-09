#include "sendballtoregion.h"
#include "include/config/trace.h"

SendBallToRegion::SendBallToRegion(const ParameterList & list)
	: kickball(nullptr)
{
	/* I'm not sure about this; in the class that uses this 
	 * (Twovone strategy), the regions are deleted after they are
	 * assigned to this. The only reason why this works is because
	 * the twovone strategy is created new each time by strategyController,
	 * and SendBallToRegion is performed only when those region pointers
	 * are active. However, if SendBallToRegion is used outside of that,
	 * there is a good chance this will crash.
	 */
    this->region = list.getParam<Region*>("region");
    state = initial;

    std::cout << "New SendBallToRegion" << std::endl;
}

SendBallToRegion::~SendBallToRegion()
{
	delete kickball;
}

void SendBallToRegion::perform(Robot * robot)
{
    #if TRACE
//        cout <<"Performing Behavior::SendBallToRegion " <<
//                 robot->getID() << endl;
    #endif

    switch (state)
    {
    case initial:
        kickball = new Skill::KickToPoint(region->centre());
        state = kick;
    case kick:
        kickball->perform(robot);
    }
}
