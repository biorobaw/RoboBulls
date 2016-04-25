#include "attacksupport.h"

AttackSupport::AttackSupport(Robot* passer)
    : main_attacker(passer)
    , wp(0,0)
    , previousAttackerPos(0,0)
    { }

void AttackSupport::perform(Robot * robot)
{
    //It's best not to have the dribbler on when waiting for a pass
    // robot->setDrible(true);

    //Reclaculates our waiting point if one side does not have the passer
    recalculateWp(robot);

    //Sets movement to always face the ball, sitting at wait point (wp)
    setMovementTargets(wp, Measurements::angleBetween(robot, main_attacker));
    StaticMovementBehavior::perform(robot);
}

void AttackSupport::recalculateWp(Robot* robot)
{
    Point gp = gameModel->getOpponentGoal();
    int goal_dir = gp.x/abs(gp.x);

    //Regions in which to check for robots
    //TODO: Use standardized regions
    static Rectangle left_of_main (0, 0, goal_dir*3000, 2000);
    static Rectangle right_of_main(0, 0, goal_dir*3000, -2000);
    static Rectangle penalty_area (gp.x, -500, gp.x-goal_dir*500, 500);

    //We do not recalculate if the attacker hasn't moved too much
    if (Measurements::isClose(main_attacker,previousAttackerPos,40))
        return;
    previousAttackerPos = main_attacker->getPosition();

    /* Logic to decide which half to stay on.
     * First, we try to go to the Region that the passer is not in.
     * If the attacker is on none, we choose the one with the least number of opponents. */
    if(left_of_main.contains(main_attacker->getPosition())) {
        wp = right_of_main.centre();
    } else if (right_of_main.contains(main_attacker->getPosition())) {
        wp = left_of_main.centre();
    }
    else {
        //Vectors of robots to ignore (itself + opponents in penalty area)
        std::vector<Robot*> ignoreOpponents, ignoreTeammates;
        ignoreTeammates.push_back(robot);
        //Filter out all robots in the penalty area from population consideration
        for(Robot * rob : gameModel->getOponentTeam()) {
            if(penalty_area.contains(rob->getPosition()))
                ignoreOpponents.push_back(rob);
        }
        if( left_of_main.numOfRobots(ignoreOpponents,ignoreTeammates) <=
           right_of_main.numOfRobots(ignoreOpponents,ignoreTeammates)) {
            wp = left_of_main.centre();
        } else {
            wp = right_of_main.centre();
        }
    }
}

Point AttackSupport::getCurrentTarget()
{
    return wp;
}
