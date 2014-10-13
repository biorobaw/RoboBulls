#include "driveballandkick.h"
#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/driveball.h"
#include "skill/stop.h"
#include "skill/kick.h"
#include "movement/gotopositionwithorientation.h"
#include "behavior/genericmovementbehavior.h"
#include "utilities/point.h"
#include <math.h>

#if SIMULATED
    #define CLOSE_ENOUGH 110
    #define ANGLE 10 * M_PI/180
    #define DIST 20
#else
    #define CLOSE_ENOUGH 250
    #define ANGLE 20 * M_PI/180
    #define DIST 20
#endif

DriveBallAndKick::DriveBallAndKick(const ParameterList& list)
	: skill(nullptr)
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
		delete skill;
        skill = new Skill::DriveBall(kickPoint, direction);
        break;
    case driving:
        cout << "in switch driving!"<<endl;

        if (Measurments::isClose(kickPoint, robot->getRobotPosition(), CLOSE_ENOUGH) &&
            Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH) &&
            abs(Measurments::angleDiff(robot->getOrientation(), direction)) < ANGLE/2 &&
            Measurments::angleDiff(robot->getOrientation(), abs(Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) <= ANGLE/4*5)
        {
            cout<<"first kick try"<<endl;
            state = kicking;
			delete skill;
            skill = new Skill::Kick();
        }
        else if (Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH)
                 && abs(Measurments::angleDiff(robot->getOrientation(), direction)) < ANGLE
                 && abs(Measurments::angleDiff(robot->getOrientation(), abs(Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint())))) > ANGLE/4*5
                 && Measurments::isClose(robot->getRobotPosition(), kickPoint, CLOSE_ENOUGH))
        {
            move.setMovementTolerances(CLOSE_ENOUGH, ANGLE);
            move.setVelocityMultiplier(0.1);
            move.recreate(gm->getBallPoint(), direction, false);
            move.perform(robot, Movement::Type::Default);
            if (Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH) &&
                Measurments::angleDiff(robot->getOrientation(), direction) < ANGLE)
            {
                state = kicking;
                delete skill;
                skill = new Skill::Kick();
            }
            else
            {
                state = driving;
            }
        }

        break;
    case kicking:
        cout << "in switch kicking!"<<endl;
        if (Measurments::isClose(gm->getBallPoint(), robot->getRobotPosition(), CLOSE_ENOUGH))
        {
                state = kicking;
                delete skill;
                skill = new Skill::Kick();
        }\
        else
        {
            state = idling;
            delete skill;
            skill = new Skill::Stop();
        }
        break;
    case idling:
        cout << "in switch idling!"<<endl;
        if (kickPoint.x > gm->getBallPoint().x)
        {
            state = driving;
			delete skill;
            skill = new Skill::DriveBall(kickPoint, direction);
        }
        else if (Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH/2)
                 && abs(Measurments::angleDiff(robot->getOrientation(), direction)) < ANGLE
                 && Measurments::angleDiff(robot->getOrientation(), abs(Measurments::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) <= ANGLE/4*5)
        {
            state = kicking;
			delete skill;
            skill = new Skill::Kick();
        }


        break;
    }

    // Perform the skill
    skill->perform(robot);
}
