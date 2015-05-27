#ifndef SENDBALLTOREGION_H
#define SENDBALLTOREGION_H

#include "behavior.h"
#include "utilities/region.h"
#include "model/robot.h"


//Right now it just makes the robot kick the ball
//to the specified region.
//More sophistication can be added later

class SendBallToRegion:public Behavior
{
public:
    SendBallToRegion(Region region);
   ~SendBallToRegion();
    void perform(Robot *);
private:
    Region region;
    Skill::Skill* kickball = nullptr;
    enum {initial,kick} state;
};


#endif // SENDBALLTOREGION_H
