#ifndef GOTOPOSITIONWITHORIENTATION_CPP
#define GOTOPOSITIONWITHORIENTATION_CPP

#include "movement/movetype.h"
#include "movement/gotopositionwithorientation.h"

namespace Movement
{

/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/

void GoToPosition::calculateVels(Robot *rob, Point targetPoint, float targetAngle, Type moveType)
{
    switch(rob->type())
    {
    case differential:
        {
            if(!IS_DIFFERENTIAL(moveType))
                moveType = ::Movement::Type::Default;
            wheelvelocities vels  = dc.calculateVels(rob, targetPoint, targetAngle, moveType);
            this->left  = vels.left;
            this->right = vels.right;
        }
        break;
    case threeWheelOmni: 
        {
            if(!IS_OMNI(moveType))
                moveType = ::Movement::Type::Default;
            threeWheelVels vels = twc.calculateVels(rob, targetPoint, targetAngle, moveType);
            this->left  = vels.L;
            this->right = vels.R;
            this->back  = vels.B;
        }
        break;
    case fourWheelOmni:
        {
            if(!IS_OMNI(moveType))
                moveType = ::Movement::Type::Default;
            fourWheelVels vels = fwc.calculateVels(rob, targetPoint, targetAngle, moveType);
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
