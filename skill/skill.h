#ifndef SKILL_H
#define SKILL_H

#include "model/robot.h"
#include "communication/robcomm.h"

namespace Skill
{


class Skill
{
public:
    virtual void perform(Robot* robot) = 0;
};


}

#endif // SKILL_H
