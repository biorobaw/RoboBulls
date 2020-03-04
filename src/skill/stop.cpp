#include "skill/stop.h"

namespace Skill
{

bool Stop::perform(Robot* robot)
{
    // Set Left Front and Left Back Motors
    robot->getPilot()->setManualVelocity(Point(0,0),0);
    return true;
}

}
