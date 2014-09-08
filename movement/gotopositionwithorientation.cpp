#ifndef GOTOPOSITIONWITHORIENTATION_CPP
#define GOTOPOSITIONWITHORIENTATION_CPP

#include "utilities/measurments.h"
#include "model/gamemodel.h"
#include "movement/movetype.h"
#include "movement/differential/differential_velcalculator.h"
#include "movement/three_omni_motion/omni3_velcalculator.h"
#include "movement/four_omni_motion/omni4_velcalculator.h"
#include "movement/gotopositionwithorientation.h"

namespace Movement
{

GoToPosition::GoToPosition
    (Point targetPoint, float targetAngle, bool withObstacleAvoid)
    : Move(targetPoint, targetAngle, withObstacleAvoid)
    {}


GoToPosition::GoToPosition
    (float tx, float ty, float targetAngle, bool withObstacleAvoid)
    : Move(Point(tx,ty), targetAngle, withObstacleAvoid)
    {}

/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/

void GoToPosition::calculateVels
    (Robot *rob, Point targetPoint, float targetAngle, Type moveType)
{
    switch(rob->type())
    {
    case differential:
        {
            if(!IS_DIFFERENTIAL(moveType))
                moveType = ::Movement::Type::Default;
            DifferentialCalculator dc;
            wheelvelocities vels 
                = dc.calculateVels(rob, targetPoint, targetAngle, moveType);
            this->left  = vels.left;
            this->right = vels.right;
        }
        break;
    case threeWheelOmni: 
        {
            if(!IS_OMNI(moveType))
                moveType = ::Movement::Type::Default;
            ThreeWheelCalculator twc;
            threeWheelVels vels 
                = twc.calculateVels(rob, targetPoint, targetAngle, moveType);
            this->left  = vels.L;
            this->right = vels.R;
            this->back  = vels.B;
        }
        break;
    case fourWheelOmni:
        {
            if(!IS_OMNI(moveType))
                moveType = ::Movement::Type::Default;
            FourWheelCalculator fwc;
            fourWheelVels vels 
                = fwc.calculateVels(rob, targetPoint, targetAngle, moveType);
            this->lfront = vels.LF;
            this->lback  = vels.LB;
            this->rfront = vels.RF;
            this->rback  = vels.RB;
        }
        break;
    }
}

}

#endif
