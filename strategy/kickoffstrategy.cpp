#include "kickoffstrategy.h"
#include "behavior/behaviorassignment.h"

KickOffStrategy::KickOffStrategy()
{
}

void KickOffStrategy::assignBeh()
{

}

bool KickOffStrategy::update()
{
    // Get information
    gm = GameModel::getModel();
    bp = gm->getBallPoint();
    float goal_x = gm->getOpponentGoal().x;

    for (Robot * robot : gm->getMyTeam())
    {
        // Define movement target for every robot
        float ball_direction = Measurments::angleBetween(robot->getRobotPosition(),bp);
        switch(robot->getID())
        {
            case 0:
                move_point0 = Point(goal_x*0.1,500);
                orientation0 = -M_PI/2;
                move_skill0.recreate(move_point0, orientation0, true);
                move_skill0.perform(robot);
                break;
            case 1:
                move_point1 = Point(goal_x*0.1,-500);
                orientation1 = M_PI/2;
                move_skill1.recreate(move_point1, orientation1, true);
                move_skill1.perform(robot);
                break;
            case 2:
                move_point2 = Point(goal_x/2,800);
                orientation2 = ball_direction;
                move_skill2.recreate(move_point2, orientation2, true);
                move_skill2.perform(robot);
                break;
            case 3:
                move_point3 = Point(goal_x/2,0);
                orientation3 = ball_direction;
                move_skill3.recreate(move_point3, orientation3, true);
                move_skill3.perform(robot);
                break;
            case 4:
                move_point4 = Point(goal_x/2,-800);
                orientation4 = ball_direction;
                move_skill4.recreate(move_point4, orientation4, true);
                move_skill4.perform(robot);
                break;
            case 5:
                move_point5 = Point(goal_x*0.9,0);
                orientation5= ball_direction;
                move_skill5.recreate(move_point5, orientation5, true);
                move_skill5.perform(robot,Movement::Type::facePoint);
        }
    }
    return false;
}
