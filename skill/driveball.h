#ifndef DRIVEBALL_H
#define DRIVEBALL_H

#include "skill/skill.h"
#include "utilities/point.h"
#include "model/robot.h"
#include "movement/move.h"

namespace Skill {

    class DriveBall:public Skill
    {
    public:
        DriveBall(Point, double);
		~DriveBall();
        bool perform(Robot*) override;
    private:
        Point targetPosition;
        double direction;
        enum states {initial, moveBehindBall, moveTowardBall, driveBall, haveTheBall} state;
        Movement::Move* skill;
        Point behindBall;
        Point closeToBall;
        Point goal;

    };
}

#endif // DRIVEBALL_H
