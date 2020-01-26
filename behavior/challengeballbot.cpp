#include "challengeballbot.h"

ChallengeBallBot::ChallengeBallBot()
{
}

void ChallengeBallBot::perform(Robot *robot)
{
    Robot* ballBot = gameModel->getHasBall();

    if(ballBot && ballBot->getTeam() != robot->getTeam())
    {
//        std::cout << "block" << std::endl;

        Point bp = gameModel->getBallPoint();
        float oppAng2Ball = Measurements::angleBetween(ballBot, bp);
        Point lead = Point(cos(oppAng2Ball), sin(oppAng2Ball)) * ROBOT_RADIUS * 3;
        Point target = bp + lead;

        float myAng2Opp = Measurements::angleBetween(robot, ballBot);
        setMovementTargets(target, myAng2Opp);
    }
    else
    {
        //Case to stop robot from floating
//        std::cout << "stop" << std::endl;
        setMovementTargets(robot->getPosition());
    }

    GenericMovementBehavior::perform(robot);
}
