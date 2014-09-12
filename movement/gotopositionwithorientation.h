#ifndef GOTOPOSITIONWITHORIENTATION_H
#define GOTOPOSITIONWITHORIENTATION_H

#include "model/robot.h"
#include "movement/move.h"
#include "movement/differential/differential_velcalculator.h"
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "movement/three_omni_motion/omni3_velcalculator.h"

namespace Movement 
{

/* GoToPosition
 * This is a generic movement layer, fit for direct use by skills.
 * It takes into account all three types of robots, and calls their 
 * corresponding Velocity Calculators. This is a specialization of the Move
 * class... defining a calculateVels function.
 */

class GoToPosition : public Move
{
public:
    GoToPosition();
   ~GoToPosition();

    GoToPosition(Point targetPoint,  float targetAngle = UNUSED_ANGLE_VALUE, 
        bool withObstacleAvoid = true);
        
    GoToPosition(float tx, float ty, float targetAngle = UNUSED_ANGLE_VALUE, 
        bool withObstacleAvoid = true);

protected:
    void calculateVels
        (Robot *rob, Point targetPoint, float targetAngle, Type moveType) override;

private:
    DifferentialCalculator dc;
    ThreeWheelCalculator   twc;
    FourWheelCalculator    fwc;
};


/* Legacy name typedef */
typedef GoToPosition GoToPositionWithOrientation;


}

#endif // GOTOPOSITIONWITHORIENTATION_H
