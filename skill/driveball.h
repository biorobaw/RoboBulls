#ifndef DRIVEBALL_H
#define DRIVEBALL_H

#include "skill/skill.h"
#include "utilities/point.h"
#include "model/robot.h"
#include "movement/gotoposition.h"

namespace Skill {

    class DriveBall:public Skill
    {
    public:
        DriveBall(Point, double);
        bool perform(Robot*) override;
    private:
        Point  targetPosition;
        double direction;
        enum states {moveBehindBall, driveBall} state;
        Movement::GoToPosition move_skill;
    };
}

#endif // DRIVEBALL_H
