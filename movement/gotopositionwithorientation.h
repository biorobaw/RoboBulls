#ifndef GOTOPOSITIONWITHORIENTATION_H
#define GOTOPOSITIONWITHORIENTATION_H
#include "model/robot.h"
#include "movement/move.h"
#include "movement/differential/differential_velcalculator.h"
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "movement/three_omni_motion/omni3_velcalculator.h"

namespace Movement 
{

/*! @brief Interface over movement calculators implementing Move::calculateVels.
 *  @author JamesW
 *  @details This class it fit for use by Skills directly for generic movement on a robot.
 *  @see Move
 *  @see FourWheelCalculator */

class GoToPosition : public Move
{
public:
    using Move::Move;

protected:
    void calculateVels(Robot *rob, Point targetPoint, float targetAngle, Type moveType) override;

private:
    /* Used to calculate a potential field to "push" robots away from
     * the goal areas if they are too close */
    fourWheelVels calculateGoalField(Robot *robot, Type moveType);

    DifferentialCalculator dc;
    ThreeWheelCalculator   twc;
    FourWheelCalculator    fwc;
};

/* Legacy name typedef */
typedef GoToPosition GoToPositionWithOrientation;

}

#endif // GOTOPOSITIONWITHORIENTATION_H
