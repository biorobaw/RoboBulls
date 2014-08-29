#include "driveballandkick.h"
#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/driveball.h"
#include "skill/basic_movement.h"
#include "skill/kick.h"
#include "skill/gotopositionwithorientation.h"
#include "utilities/point.h"
#include "include/globals.h"
#include <math.h>

#define CLOSE_ENOUGH 200
#define ANGLE 20 * M_PI/180
#define DIST 20

DriveBallAndKick::DriveBallAndKick(const ParameterList& list)
{
    UNUSED_PARAM(list);
    state = initial;
    cout<<"initial driving!"<<endl;

}

void DriveBallAndKick::perform(Robot* robot)
{
    GameModel* gm = GameModel::getModel();

    Point goal = gm->getOpponentGoal();
    Point kickPoint(1600, 0);
    double direction = Measurments::angleBetween(kickPoint, goal);

    // Create a different skill depending on the state
    switch (state)
    {
    case initial:
        cout<<"drive ball and kick initial state"<<endl;
        state = driving;
        skill = new Skill::DriveBall(kickPoint, direction);
        break;
    case driving:
        cout << "in switch driving!"<<endl;
//        ballPoint = new Point(gm->getBallPoint().x, gm->getBallPoint().y);
        if (Measurments::isClose(kickPoint, robot->getRobotPosition(), CLOSE_ENOUGH) &&
            Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH) &&
            abs(Measurments::angleDiff(robot->getOrientation(), direction)) < ANGLE/2 &&
            Measurments::angleDiff(robot->getOrientation(), abs(Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) <= ANGLE/4*5)
        {
            cout<<"first kick try"<<endl;
            state = kicking;
            skill = new Skill::Kick();
        }
//        else if (Measurments::isClose(robot->getRobotPosition(), kickPoint, CLOSE_ENOUGH/2) &&
//                 Measurments::angleDiff(robot->getOrientation(), abs(Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) > ANGLE/2)
//        {
//            cout << "second kick try!"<<endl;
//            behindBall = new Point(DIST*5*cos(Measurments::angleBetween(goal,gm->getBallPoint()))+gm->getBallPoint().x,
//                                   DIST*5*sin(Measurments::angleBetween(goal,gm->getBallPoint()))+gm->getBallPoint().y);
//            skill = new Skill::GoToPositionWithOrientation (*behindBall, Measurments::angleBetween(gm->getBallPoint(), goal));
//            if (Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH) &&
//                Measurments::angleDiff(robot->getOrientation(), Measurments::angleBetween(gm->getBallPoint(), goal)) < ANGLE)
//            {
//                state = kicking;
//                skill = new Skill::Kick();
//            }
//        }

        break;
    case kicking:
        cout << "in switch kicking!"<<endl;
        state = idling;
        skill = new Skill::Stop();
        break;
    case idling:
        cout << "in switch idling!"<<endl;
        if (kickPoint.x > gm->getBallPoint().x)
        {
            state = driving;
            skill = new Skill::DriveBall(kickPoint, direction);
        }
        else if (Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH/2)
                 && abs(Measurments::angleDiff(robot->getOrientation(), direction)) < ANGLE
                 && Measurments::angleDiff(robot->getOrientation(), abs(Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) <= ANGLE/4*5)
        {
            state = kicking;
            skill = new Skill::Kick();
        }

        break;
    }

    // Perform the skill
    skill->perform(robot);

}
