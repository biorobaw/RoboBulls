#include "positionforkickoff.h"

PositionForKickoff::PositionForKickoff(const ParameterList &list)
{
    UNUSED_PARAM(list);
}

PositionForKickoff::~PositionForKickoff()
{
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

    move_skill.recreate(move_point, orientation, false);
    if (robot->getID()==5)
        move_skill.perform(robot,Movement::Type::facePoint);
    else
        move_skill.perform(robot);
}
