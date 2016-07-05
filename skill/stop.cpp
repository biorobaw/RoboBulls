#include "skill/stop.h"

namespace Skill
{

bool Stop::perform(Robot* robot)
{
    // Set Left Front and Left Back Motors
    robot->setL(0);

    // Set Right Front and Right Back Motors
    robot->setR(0);
    return true;
}

}
