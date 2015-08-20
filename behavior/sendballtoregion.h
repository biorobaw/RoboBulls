#ifndef SENDBALLTOREGION_H
#define SENDBALLTOREGION_H

#include "behavior.h"
#include "utilities/region.h"
#include "model/robot.h"

/*! @brief The robot kicks the ball to the center of a specified Region,
 * @author Muhaimen Shamsi
 * @details See Region documentation for more details on Regions. */

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
