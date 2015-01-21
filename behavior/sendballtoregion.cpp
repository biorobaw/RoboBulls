#include "sendballtoregion.h"

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
