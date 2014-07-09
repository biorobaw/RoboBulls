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

DriveBallAndKick::DriveBallAndKick(const ParameterList& list)
{
    state = initial;

    cout<<"initial driving!"<<endl;

}

void DriveBallAndKick::perform(Robot* robot)
{
    GameModel* gm = GameModel::getModel();

    Point goal = gm->getMyGoal();
    Point kickPoint(-1500,0);

    double direction = Measurments::angleBetween(goal, kickPoint);


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
        if (Measurments::isClose(kickPoint, robot->getRobotPosition(), 110) && Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110))
        {
            if (abs(Measurments::angleDiff(robot->getOrientation(), direction)) < 10 * M_PI/180)
            {
                state = kicking;
                skill = new Skill::Kick();
            }
            else
            {
                state = finalOrientationFixing;
                skill = new Skill::GoToPositionWithOrientation(kickPoint, direction);
            }
        }
        break;
    case kicking:
        cout << "in switch kicking!"<<endl;
        state = idling;
        skill = new Skill::Stop();
        break;
    case idling:
        cout << "in switch idling!"<<endl;
        if (kickPoint.x < gm->getBallPoint().x && abs(robot->getRobotPosition().x - gm->getBallPoint().x) > 110 )
        {
            state = driving;
            skill = new Skill::DriveBall(kickPoint, direction);
        }
        if (Measurments::isClose(robot->getRobotPosition(), gm->getBallPoint(), 110))
        {
            state = kicking;
            skill = new Skill::Kick();
        }
        break;
    case finalOrientationFixing:
        cout<<"Fixing orientation before kicking!"<<endl;
        if (abs(Measurments::angleDiff(robot->getOrientation(), direction)) < 10 * M_PI/180)
        {
            state = kicking;
            skill = new Skill::Kick();
        }
        break;
    }

    // Perform the skill
    skill->perform(robot);

}
