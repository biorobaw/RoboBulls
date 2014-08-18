#include "sendballtoregion.h"

SendBallToRegion::SendBallToRegion(const ParameterList & list)
{
    this->region = list.getParam<Region*>("region");
    state = initial;
}

void SendBallToRegion::perform(Robot * robot)
{
    #if TRACE
        cout << "Performing Behavior::SendBallToRegion" << endl;
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
