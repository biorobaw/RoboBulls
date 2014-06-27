#ifndef DRIVEBALL_H
#define DRIVEBALL_H

#include "skill/skill.h"
#include "utilities/point.h"
#include "model/robot.h"

namespace Skill{

    class DriveBall:public Skill
    {
    public:
        DriveBall(Point, double);
        bool perform(Robot*);
    private:
        double direction;
        enum states {initial, moveTowardBall, driveBall, idiling} state;
        Skill * skill;

    };
}

#endif // DRIVEBALL_H
