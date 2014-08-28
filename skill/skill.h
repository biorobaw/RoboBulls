#ifndef SKILL_H
#define SKILL_H

#include "model/robot.h"
#include "communication/robcomm.h"

namespace Skill
{


class Skill
{
public:
    virtual bool perform(Robot* robot) = 0;
    virtual ~Skill(){}
};


}

#endif // SKILL_H
