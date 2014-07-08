#ifndef MYTESTBEHAVIOR_H
#define MYTESTBEHAVIOR_H
#include <iostream>
#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "utilities/paramlist.h"

class myTestBehavior: public Behavior
{
public:
    myTestBehavior(const ParameterList &list);
    void perform(Robot *);
//private:
//    Point TargetPoint;
};

#endif // MYTESTBEHAVIOR_H

