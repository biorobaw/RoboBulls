#ifndef SKILL_H
#define SKILL_H

#include "model/robot.h"
#include "communication/robcomm.h"
#include "utilities/paramlist.h"

namespace Skill
{


class Skill
{
public:
    virtual void perform(Robot* robot) = 0;
};


}

#endif // SKILL_H
