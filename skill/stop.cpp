#include "skill/stop.h"

namespace Skill
{

bool Stop::perform(Robot* robot)
{
    robot->setL(0);
    robot->setR(0);
    return true;
}

}
