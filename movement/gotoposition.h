#ifndef GOTOPOSITON_H
#define GOTOPOSITON_H
#include "model/robot.h"
#include "movement/move.h"
#include "movement/differential/differential_velcalculator.h"
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "movement/three_omni_motion/omni3_velcalculator.h"

namespace Movement
{

/*! @brief Top-level Interface to provide robot movement
 *  @author JamesW
 *  @details This class it fit for use by Skills directly for generic movement on a robot.
 *  @see Move
 *  @see FourWheelCalculator */

class GoToPosition : public Move
{
public:
    using Move::Move;

protected:
    /*! @brief Uses one of the three type calculators to calculate velocities for the robot.
     * @details The calculated velocities are stored in *this, inheiting from move.
     * setVels is then called so update the number in Robot.
     * @param rob Robot to calculate for
     * @param targetPoint The final target point to move to
     * @param targetAngle The final target angle to be facing
     * @param moveType The Movement Type. See Type
     * @see DifferentialCalculator
     * @see ThreeWheelCalculator
     * @see FourWheelCalculator */
    void calculateVels(Robot *rob, Point targetPoint, float targetAngle, Type moveType) override;

private:
    /* Used to calculate a potential field to "push" robots away from
     * the goal areas if they are too close */
    fourWheelVels calculateGoalField(Robot *robot, Type moveType);

    DifferentialCalculator dc;
    ThreeWheelCalculator   twc;
    FourWheelCalculator    fwc;
};

}

#endif
