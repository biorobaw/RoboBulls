#include "positionforkickoff.h"

PositionForKickoff::PositionForKickoff(const ParameterList &list)
<<<<<<< HEAD
=======
	: move_skill(nullptr)
>>>>>>> 67f6d320b6a48c12f5f11751171467ab5d2d2697
{
    UNUSED_PARAM(list);
}

PositionForKickoff::~PositionForKickoff()
{
<<<<<<< HEAD
=======
	delete move_skill;
>>>>>>> 67f6d320b6a48c12f5f11751171467ab5d2d2697
}

void PositionForKickoff::perform(Robot * robot)
{
    #if TRACE
        cout << endl<<"Performing Behavior::PositionForKickoff" << endl;
    #endif

    gm = GameModel::getModel();
    bp = gm->getBallPoint();
    float ball_direction = Measurments::angleBetween(robot->getRobotPosition(),bp);
    float goal_x = gm->getMyGoal().x;

    switch(robot->getID())
    {
        case 0:
            move_point = Point(goal_x*0.1,500);
            orientation= -M_PI/2;
            break;
        case 1:
            move_point = Point(goal_x*0.1,-500);
            orientation= M_PI/2;
            break;
        case 2:
            move_point = Point(goal_x/2,800);
            orientation= ball_direction;
            break;
        case 3:
            move_point = Point(goal_x/2,0);
            orientation= ball_direction;
            break;
        case 4:
            move_point = Point(goal_x/2,-800);
            orientation = ball_direction;
            break;

        case 5:
            move_point = Point(goal_x*0.8,0);
            orientation= ball_direction;
    }

<<<<<<< HEAD
    move_skill.recreate(move_point, orientation, false);
    if (robot->getID()==5)
        move_skill.perform(robot,Movement::Type::facePoint);
    else
        move_skill.perform(robot);
=======
    if(bp_updated)
    {
		delete move_skill;
        #if SIMULATED==1
            move_skill = new Movement::GoToPosition(move_point, orientation, true);
        #else
            move_skill = new Movement::GoToPosition(move_point, orientation, true);
        #endif

        bp_updated = false;
        //cout << "BP UPDATED" << endl;
    }

    move_skill->perform(robot);
>>>>>>> 67f6d320b6a48c12f5f11751171467ab5d2d2697
}
