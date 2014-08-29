#include "attacksupport.h"


AttackSupport::AttackSupport(const ParameterList& list)
{
    UNUSED_PARAM(list);
    state = initial;
}

void AttackSupport::perform(Robot * robot)
{
    #if TRACE
        cout <<endl<< "Performing Behavior::AttackSupport" << endl;
    #endif

    GameModel * gm = GameModel::getModel();

    //Set either robID(1) or robID(2) as the support_attacker
//    Robot * main_attacker;
//    if(robot->getID()==0)
//        main_attacker = gm->getMyTeam().at(1);
//    else
//        main_attacker = gm->getMyTeam().at(0);


    //Get info from gamemodel
    Point rp = robot->getRobotPosition();
    Point gp = gm->getOpponentGoal();
    Point bp = gm->getBallPoint();
    int goal_dir = gp.x/abs(gp.x);

    //Declare regions in which to check for robots
    Region left_of_main = Region(0, goal_dir*3000, 0, 2000);
    Region right_of_main = Region(0, goal_dir*3000, 0, -2000);
    Region penalty_area = Region(gp.x, gp.x-goal_dir*500,-500,500);



    //Vectors of robots to ignore (itself + opponents in penalty area)
    vector<Robot*> ignoreOpponents, ignoreTeammates;

    ignoreTeammates.push_back(robot);

    for(Robot * rob : gm->getOponentTeam())
    {
        if(penalty_area.contains(rob->getRobotPosition()))
        {
            ignoreOpponents.push_back(rob);
        }
    }

    //Initialize wp which is the point at which to wait for pass
    //wp is set on the side that is least populated - not counting enemy robots in penalty area
    Point wp;
    if(left_of_main.numOfRobots(ignoreOpponents,ignoreTeammates)< right_of_main.numOfRobots(ignoreOpponents,ignoreTeammates))
        wp = left_of_main.centre();
    else if (left_of_main.numOfRobots(ignoreOpponents,ignoreTeammates) > right_of_main.numOfRobots(ignoreOpponents,ignoreTeammates))
        wp = right_of_main.centre();
    else
        wp = Point(0,0);

    //Initialize skills that are used in switch statement
    float angle_to_ball = Measurments::angleBetween(rp,bp);

    switch (state)
    {
    case initial:
    {
    #if SIMULATED
        move_skill = new Skill::ObstacleAvoidMove(wp,angle_to_ball);
        previousBP = bp;
    #else
        move_skill = new Skill::ObstacleAvoidMove(wp,angle_to_ball);
        previousBP = bp;
    #endif
        state = final;
        break;
    }

    case final:
        if (!Measurments::isClose(bp,previousBP,100))
        {
            state = initial;
            break;
        }
        move_skill->perform(robot);
    }
}
