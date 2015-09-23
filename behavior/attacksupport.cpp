#include "attacksupport.h"

AttackSupport::AttackSupport(Robot* passer)
    : main_attacker(passer)
    , wp(0,0)
    , previousBP(0,0)
    { }

void AttackSupport::perform(Robot * robot)
{
    //It's best to have the dribbler on when waiting for a pass
    robot->setDrible(true);

    //Reclaculates our waiting point if one side is less populated
    recalculateWp(robot);

    //Sets movement to always face the ball, sitting at wait point (wp)
    setMovementTargets(wp, Measurments::angleBetween(robot,gameModel->getBallPoint()));
    GenericMovementBehavior::perform(robot);
}

void AttackSupport::recalculateWp(Robot* robot)
{
    Point gp = gameModel->getOpponentGoal();
    Point bp = gameModel->getBallPoint();
    int goal_dir = gp.x/abs(gp.x);

    //Regions in which to check for robots
    static Region left_of_main (0, goal_dir*3000, 0, 2000);
    static Region right_of_main(0, goal_dir*3000, 0, -2000);
    static Region penalty_area (gp.x, gp.x-goal_dir*500,-500,500);

    //Re do not recalculate if the ball hasn't moved too much
    if (Measurments::isClose(bp,previousBP,100))
        return;
    previousBP = bp;

    //Vectors of robots to ignore (itself + opponents in penalty area)
    std::vector<Robot*> ignoreOpponents, ignoreTeammates;
    ignoreTeammates.push_back(robot);

    //Filter out all robots in the penalty area from population consideration
    for(Robot * rob : gameModel->getOponentTeam()) {
        if(penalty_area.contains(rob->getRobotPosition()))
            ignoreOpponents.push_back(rob);
    }

    //wp is set on the side that is least populated - not counting enemy robots in penalty area
    if(left_of_main.numOfRobots(ignoreOpponents,ignoreTeammates) <=
       right_of_main.numOfRobots(ignoreOpponents,ignoreTeammates) and
      !left_of_main.contains(main_attacker->getRobotPosition()))
        wp = left_of_main.centre();
    else if (!right_of_main.contains(main_attacker->getRobotPosition()))
        wp = right_of_main.centre();
    else
        wp = Point(goal_dir * 500, 0);
}

Point AttackSupport::getCurrentTarget()
{
    return wp;
}
