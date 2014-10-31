#ifndef MYTESTBEHAVIOR_H
#define MYTESTBEHAVIOR_H
#include <iostream>
#include "behavior/behavior.h"
#include "skill/skill.h"
#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "model/robot.h"
#include "utilities/paramlist.h"
#include "movement/gotopositionwithorientation.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

class myTestBehavior: public GenericMovementBehavior/*public Behavior*/
{
public:
    myTestBehavior(const ParameterList &list);
    void perform(Robot *);
private:
//    Point TargetPoint;
    Skill::Skill *mySkill;
    Movement::GoToPosition move;
};

#endif // MYTESTBEHAVIOR_H

