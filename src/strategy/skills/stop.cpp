#include "stop.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"

namespace Skill
{

    bool Stop::perform(Robot* robot)
    {
        // Set Left Front and Left Back Motors
        robot->getPilot()->setManualVelocity(Point(0,0),0);
        return true;
    }

}
