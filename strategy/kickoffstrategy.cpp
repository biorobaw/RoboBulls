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

    float goal_x = gm->getMyGoal().x;

    cout << gm->getBallPoint().toString() << endl;

    for (Robot * robot : gm->getMyTeam())
    {
        // Define movement target for every robot
        float ball_direction = Measurments::angleBetween(robot->getRobotPosition(),bp);
        switch(robot->getID())
        {
            case 0:
                move_skill0.recreate(Point(goal_x*0.1,500), -M_PI/2, true);
                move_skill0.perform(robot);
                break;
            case 1:
                move_skill1.recreate(Point(goal_x*0.1,-500), M_PI/2, true);
                move_skill1.perform(robot);
                break;
            case 4:
                move_skill2.recreate(Point(goal_x/2,800), ball_direction, true);
                move_skill2.perform(robot, Movement::Type::Default);
                break;
            case 3:
                move_skill3.recreate(Point(goal_x/2,0), ball_direction, true);
                move_skill3.perform(robot);
                break;
            case 2:
                move_skill4.recreate(Point(goal_x/2,-800), ball_direction, true);
                move_skill4.perform(robot);
                break;
            case 5:
                move_skill5.recreate(Point(goal_x*0.9,0), ball_direction, true);
                move_skill5.perform(robot,Movement::Type::facePoint);
        }
    }
    return false;
}
