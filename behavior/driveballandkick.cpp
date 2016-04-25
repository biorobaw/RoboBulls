#include "driveballandkick.h"
#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/driveball.h"
#include "skill/stop.h"
#include "skill/kick.h"
#include "movement/gotoposition.h"
#include "behavior/genericmovementbehavior.h"
#include "utilities/point.h"
#include <math.h>
using namespace std;

#if SIMULATED
    #define CLOSE_ENOUGH 110
    #define ANGLE 10 * M_PI/180
    #define DIST 20
#else
    #define CLOSE_ENOUGH 250
    #define ANGLE 20 * M_PI/180
    #define DIST 20
#endif

DriveBallAndKick::DriveBallAndKick()
    : skill(nullptr)
{
    state = initial;
    cout<<"initial driving!"<<endl;
}

void DriveBallAndKick::perform(Robot* robot)
{
    GameModel* gm = GameModel::getModel();

    Point goal = gm->getOpponentGoal();
    Point kickPoint(1600, 0);
    double direction = Measurements::angleBetween(kickPoint, goal);

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

        if (Measurements::isClose(kickPoint, robot->getPosition(), CLOSE_ENOUGH) &&
            Measurements::isClose(robot->getPosition(), gm->getBallPoint(), CLOSE_ENOUGH) &&
            abs(Measurements::angleDiff(robot->getOrientation(), direction)) < ANGLE/2 &&
            Measurements::angleDiff(robot->getOrientation(), abs(Measurements::angleBetween(robot->getPosition(), gm->getBallPoint()))) <= ANGLE/4*5)
        {
            cout<<"first kick try"<<endl;
            state = kicking;
            delete skill;
            skill = new Skill::Kick();
        }
        else if (Measurements::isClose(robot->getPosition(), gm->getBallPoint(), CLOSE_ENOUGH)
                 && abs(Measurements::angleDiff(robot->getOrientation(), direction)) < ANGLE
                 && abs(Measurements::angleDiff(robot->getOrientation(), abs(Measurements::angleBetween(robot->getPosition(), gm->getBallPoint())))) > ANGLE/4*5
                 && Measurements::isClose(robot->getPosition(), kickPoint, CLOSE_ENOUGH))
        {
            move.setMovementTolerances(CLOSE_ENOUGH, ANGLE);
            move.setVelocityMultiplier(0.1);
            move.recreate(gm->getBallPoint(), direction, true);
            move.perform(robot, Movement::Type::Default);
            if (Measurements::isClose(robot->getPosition(), gm->getBallPoint(), CLOSE_ENOUGH) &&
                Measurements::angleDiff(robot->getOrientation(), direction) < ANGLE)
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
        if (Measurements::isClose(gm->getBallPoint(), robot->getPosition(), CLOSE_ENOUGH))
        {
                state = kicking;
                delete skill;
                skill = new Skill::Kick();
        }
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
        else if (Measurements::isClose(robot->getPosition(), gm->getBallPoint(), CLOSE_ENOUGH/2)
                 && abs(Measurements::angleDiff(robot->getOrientation(), direction)) < ANGLE
                 && Measurements::angleDiff(robot->getOrientation(), abs(Measurements::angleBetween(robot->getPosition(), gm->getBallPoint()))) <= ANGLE/4*5)
        {
            state = kicking;
            delete skill;
            skill = new Skill::Kick();
        }
//        else if (measurements::isClose(robot->getRobotPosition(), gm->getBallPoint(), CLOSE_ENOUGH/2)
//                 && abs(measurements::angleDiff(robot->getOrientation(), direction)) < ANGLE
//                 && measurements::angleDiff(robot->getOrientation(), abs(measurements::angleBetween(robot->getRobotPosition(), gm->getBallPoint()))) > ANGLE/4*5)
//        {

//        }


        break;
    }

    // Perform the skill
    skill->perform(robot);
}
