#include "challengeballbot.h"
#include "model/ball.h"

ChallengeBallBot::ChallengeBallBot()
{
}

void ChallengeBallBot::perform(Robot *robot)
{
    Robot* ballBot = Ball::getRobotWithBall();

    if(ballBot && ballBot->getTeam() != robot->getTeam())
    {
//        std::cout << "block" << std::endl;

        Point bp = Ball::getPosition();
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
