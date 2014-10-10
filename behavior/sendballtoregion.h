#ifndef SENDBALLTOREGION_H
#define SENDBALLTOREGION_H

#include "behavior.h"
#include "skill/kicktopoint.h"
#include "utilities/region.h"
#include "model/robot.h"


//Right now it just makes the robot kick the ball
//to the specified region.
//More sophistication can be added later

class SendBallToRegion:public Behavior
{
public:
    SendBallToRegion(const ParameterList & list);
   ~SendBallToRegion();
    void perform(Robot *);
private:
    Region * region;
    Skill::KickToPoint * kickball;
    enum {initial,kick} state;
};


#endif // SENDBALLTOREGION_H
