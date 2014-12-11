#include "skill/stop.h"
#include "gui/guiinterface.h"

namespace Skill
{

bool Stop::perform(Robot* robot)
{
    if(!GuiInterface::getGuiInterface()
            ->isOverriddenBot()[robot->id]) {
        robot->setL(0);
        robot->setR(0);
    }
    return true;
}

}
