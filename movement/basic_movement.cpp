#include "movement/basic_movement.h"

namespace Movement
{

void GoForward::perform(Robot* robot)
{
	robot->setL(30);
	robot->setR(30);
}

void GoBackward::perform(Robot* robot)
{
	robot->setL(-30);
	robot->setR(-30);
}

}
