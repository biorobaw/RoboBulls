#include "challengeballbot.h"
#include "model/ball.h"
#include "model/game_state.h"
#include "robot/robot.h"
#include "robot/navigation/robot_pilot.h"
#include "model/game_state.h"
#include "model/team.h"
#include "utilities/measurements.h"

ChallengeBallBot::ChallengeBallBot(Robot* robot) : Behavior(robot)
{
}

void ChallengeBallBot::perform()
{
    Robot* ballBot = robot->getTeam()->getGameState()->getRobotWithBall();

    if(ballBot && ballBot->getTeam() != robot->getTeam())
    {
//        std::cout << "block" << std::endl;

        Point bp = robot->getTeam()->getGameState()->getBall()->getPosition();
        float oppAng2Ball = Measurements::angleBetween(ballBot, bp);
        Point lead = Point(cos(oppAng2Ball), sin(oppAng2Ball)) * ROBOT_RADIUS * 3;
        Point target = bp + lead;

        float myAng2Opp = Measurements::angleBetween(robot, ballBot);
        auto cmd = CmdGoToPose(target,myAng2Opp);
        robot->getPilot()->setNewCommand(cmd);
    }
    else
    {
        //Case to stop robot from floating
//        std::cout << "stop" << std::endl;
        auto cmd = CmdGoToPose(robot->getPosition());
        robot->getPilot()->setNewCommand(cmd);
    }
    done = robot->getPilot()->finishedCommand();
}

bool ChallengeBallBot::isFinished() {
    return done;
}

string ChallengeBallBot::getName() {
    return "ChallengeBallBot";
}

