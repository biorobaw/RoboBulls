#include "model/gamemodel.h"
#include "movement/movetype.h"
#include "movement/gotoposition.h"
#include "utilities/region/defencearea.h"

namespace Movement
{

/***********************************************************/
/******************** Protected Methods ********************/
/***********************************************************/

void GoToPosition::calculateVels(Robot *rob, Point targetPoint, float targetAngle, Type moveType)
{
    //Pushes the robot away from a goalie box if it is close, for non-goalies
    fourWheelVels fieldVels = {0,0,0,0};
//    if(rob->getID() != GOALIE_ID)
//        fieldVels = calculateGoalField(rob, moveType);

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
            this->lfront = vels.LF + fieldVels.LF;
            this->lback  = vels.LB + fieldVels.LB;
            this->rfront = vels.RF + fieldVels.RF;
            this->rback  = vels.RB + fieldVels.RB;
        }
        break;
    }
}

/***********************************************************/
/******************** Private Methods **********************/
/***********************************************************/
fourWheelVels GoToPosition::calculateGoalField(Robot* robot, Type moveType)
{
    fourWheelVels result = {0, 0, 0, 0};

    DefenceArea da0(config->our_team);
    DefenceArea da1(!config->our_team);

    Point rp = robot->getPosition();
    if(da0.contains(rp, DEF_AREA_TOL))
    {
        da0.expelPoint(rp);

        float ang_to_goal = Measurements::angleBetween(robot->getPosition(), rp);
        Point unit(cos(ang_to_goal), sin(ang_to_goal));

        //The `target` is the robot's point plus a vector away from the goal
        Point target = robot->getPosition() + (unit * 1000);
        result = fwc.calculateVels(robot, target, robot->getOrientation(), moveType);
    }
    else if(da1.contains(rp, DEF_AREA_TOL))
    {
        da1.expelPoint(rp);

        float ang_to_goal = Measurements::angleBetween(robot->getPosition(), rp);
        Point unit(cos(ang_to_goal), sin(ang_to_goal));

        //The `target` is the robot's point plus a vector away from the goal
        Point target = robot->getPosition() + (unit * 1000);
        result = fwc.calculateVels(robot, target, robot->getOrientation(), moveType);
    }

    return std::move(result);
}

}
