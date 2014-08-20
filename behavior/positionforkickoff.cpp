#include "positionforkickoff.h"

PositionForKickoff::PositionForKickoff(const ParameterList &list)
{
    bp_updated = false;
}

void PositionForKickoff::perform(Robot * robot)
{
    #if TRACE
        cout << endl<<"Performing Behavior::PositionForKickoff" << endl;
    #endif

    GameModel * gm = GameModel::getModel();

    if(gm->getBallPoint()!=bp)
    {
        bp = gm->getBallPoint();
        bp_updated = true;
    }

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
            move_point = Point(gm->getMyGoal());
            orientation= ball_direction;
    }

    if(bp_updated)
    {
        #if SIMULATED==1
            move_skill = new Skill::ObstacleAvoidMove(move_point,orientation);
        #else
            move_skill = new Skill::ObstacleAvoidMove(move_point,orientation);
        #endif

        bp_updated = false;
        //cout << "BP UPDATED" << endl;
    }

    move_skill->perform(robot);
}
