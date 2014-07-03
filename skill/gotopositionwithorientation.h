#ifndef GOTOPOSITIONWITHORIENTATION_H
#define GOTOPOSITIONWITHORIENTATION_H

#include "skill.h"
#include "model/robot.h"
#include "skill/differential_control/closedloopcontrol.h"

namespace Skill {

    class GoToPositionWithOrientation : public Skill
    {
    public:
        GoToPositionWithOrientation(Point target, double goalOrientation);
        GoToPositionWithOrientation(float tx, float ty, double goalOrientation);

        void setVelocityMultiplier(double multipier);
        bool perform(Robot * robot);
    private:
        ClosedLoopControl control;
        Point   targetPosition;
        double goalOrientation;
    };

}

#endif // GOTOPOSITIONWITHORIENTATION_H
