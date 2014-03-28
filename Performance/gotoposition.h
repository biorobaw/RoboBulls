#ifndef GOTOPOSITION_H
#define GOTOPOSITION_H

#include "skill.h"
#include "Robot/robot.h"

class GoToPosition:public Skill
{
public:
    GoToPosition();
    void perform();
};

#endif // GOTOPOSITION_H
