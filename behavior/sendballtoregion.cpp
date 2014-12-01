#include "sendballtoregion.h"
#include "include/config/trace.h"

SendBallToRegion::SendBallToRegion(Region region)
	: kickball(nullptr)
{
    this->region = region;
    state = initial;
}

SendBallToRegion::~SendBallToRegion()
{
	delete kickball;
}

void SendBallToRegion::perform(Robot * robot)
{
    #if TRACE
        cout <<"Performing Behavior::SendBallToRegion " <<
                 robot->getID() << endl;
    #endif

    switch (state)
    {
    case initial:
        kickball = new Skill::KickToPoint(region.centre());
        state = kick;
        //std::cout << "Moving to kick" <<std::endl;
    case kick:
        kickball->perform(robot);
    }
}
