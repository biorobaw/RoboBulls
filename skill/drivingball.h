#ifndef DRIVINGBALL_H
#define DRIVINGBALL_H

#include "skill/skill.h"
#include "model/robot.h"
#include "utilities/point.h"

namespace skill{

    class drivingBall : public skill
    {
    public:
        drivingBall(Robot*, Point);
        bool perform(Robot*);
    };
}

#endif // DRIVINGBALL_H
