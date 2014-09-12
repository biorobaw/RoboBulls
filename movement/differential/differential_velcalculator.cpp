#include "movement/movetype.h"
#include "movement/differential/differential_velcalculator.h"

namespace Movement
{


wheelvelocities DifferentialCalculator::calculateVels
    (Robot* rob, Point goalPoint, float theta_goal, Type moveType)
{
	return calculateVels(rob, goalPoint.x, goalPoint.y, theta_goal, moveType);
}

wheelvelocities DifferentialCalculator::calculateVels
    (Robot* rob, float x_goal, float y_goal, float theta_goal, Type moveType)
{
	wheelvelocities result;
	
	switch(moveType)
	{
    case Type::Default:
		{
			result = clc.closed_loop_control(rob, x_goal, y_goal, theta_goal);
		}
		break;
    case Type::SharpTurns:
		{
            result = clst.closed_loop_control(rob, x_goal, y_goal, theta_goal);
		}
		break;
    case Type::NoSlowdown:
		{
            result = clnsd.closed_loop_control(rob, x_goal, y_goal, theta_goal);
		}
		break;
	default:
		std::string msg = "DifferentialCalculator called with bad moveType " + 
			std::to_string((MovementType)moveType);
		throw std::runtime_error(msg.c_str());
	}
	
	return result;
}


}
