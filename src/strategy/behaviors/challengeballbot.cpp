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
        auto cmd = CmdGoToPose(target,myAng2Opp);
        robot->getPilot()->goToPose(cmd);
    }
    else
    {
        //Case to stop robot from floating
//        std::cout << "stop" << std::endl;
        auto cmd = CmdGoToPose(robot->getPosition());
        robot->getPilot()->goToPose(cmd);
    }
    done = robot->getPilot()->finisedLastCommand();
}

bool ChallengeBallBot::isFinished() {
    return done;
}

