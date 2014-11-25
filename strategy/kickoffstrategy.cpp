#include "kickoffstrategy.h"
#include "behavior/behaviorassignment.h"
#include "include/config/team.h"

#define COUNT_WAIT 500

KickOffStrategy::KickOffStrategy()
{
    for(int& i : counters)
        i = 0;
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

    for (Robot * robot : gm->getMyTeam())
    {
        // Define movement target for every robot
        float ball_direction = Measurments::angleBetween(robot->getRobotPosition(),bp);
        switch(robot->getID())
        {
            case 1:
            if ((gm->getGameState() == 'K' && TEAM == TEAM_BLUE) ||
                    (gm->getGameState() == 'k' && TEAM == TEAM_YELLOW))
            {
                move_skill0.recreate(Point(goal_x*0.2,0), ball_direction, true);
                move_skill0.perform(robot);
            }
            else if ((gm->getGameState() == 'k' && TEAM == TEAM_BLUE) ||
                     (gm->getGameState() == 'K' && TEAM == TEAM_YELLOW))
            {
                move_skill0.recreate(Point(goal_x*0.7,-700), ball_direction, true);
                if(++counters[robot->getID()] > COUNT_WAIT)
                    move_skill0.perform(robot);
            }
                break;
            case 4:
            if ((gm->getGameState() == 'K' && TEAM == TEAM_BLUE) ||
                    (gm->getGameState() == 'k' && TEAM == TEAM_YELLOW))
            {
                move_skill1.recreate(Point(goal_x*0.1,0), ball_direction, true);
                move_skill1.perform(robot);
            }
            else if ((gm->getGameState() == 'k' && TEAM == TEAM_BLUE) ||
                     (gm->getGameState() == 'K' && TEAM == TEAM_YELLOW))
            {
                move_skill1.recreate(Point(goal_x*0.7,-700), ball_direction, true);
                if(++counters[robot->getID()] > COUNT_WAIT)
                    move_skill1.perform(robot);
            }
                break;
            case 2:
            if ((gm->getGameState() == 'K' && TEAM == TEAM_BLUE) ||
                    (gm->getGameState() == 'k' && TEAM == TEAM_YELLOW))
            {
                move_skill2.recreate(Point(goal_x/2,800), ball_direction, true);
                move_skill2.perform(robot);
            }
            else if ((gm->getGameState() == 'k' && TEAM == TEAM_BLUE) ||
                     (gm->getGameState() == 'K' && TEAM == TEAM_YELLOW))
            {
                move_skill2.recreate(Point(goal_x*0.7,800), ball_direction, true);
                if(++counters[robot->getID()] > COUNT_WAIT)
                    move_skill2.perform(robot);
            }
                break;
            case 3:
            if ((gm->getGameState() == 'K' && TEAM == TEAM_BLUE) ||
                    (gm->getGameState() == 'k' && TEAM == TEAM_YELLOW))
            {
                move_skill3.recreate(Point(goal_x/2,700), ball_direction, true);
                move_skill3.perform(robot);
            }
            else if ((gm->getGameState() == 'k' && TEAM == TEAM_BLUE) ||
                     (gm->getGameState() == 'K' && TEAM == TEAM_YELLOW))
            {
                move_skill3.recreate(Point(goal_x*0.7,-700), ball_direction, true);
                if(++counters[robot->getID()] > COUNT_WAIT)
                    move_skill3.perform(robot);
            }

                break;
            case 0:
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
