#include "driveballandkick.h"
#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/driveball.h"
#include "skill/basic_movement.h"
#include "skill/kick.h"
#include "utilities/point.h"
#include "include/globals.h"
#include <math.h>

DriveBallAndKick::DriveBallAndKick(const ParameterList& list)
{
    state = driving;
    cout<<"initial driving!"<<endl;
}

void DriveBallAndKick::perform(Robot* robot)
{
    GameModel* gm = GameModel::getModel();

    Point goal(-3000, 0);
    double direction = Measurments::angleBetween(robot->getRobotPosition(), goal);

    Point rp = robot->getRobotPosition();
    Point bp = gm->getBallPoint();

    Point kickPoint(-1500,0);

    // Create a different skill depending on the state
    Skill::Skill * skill;
    switch (state)
    {
    case driving:
        skill = new Skill::DriveBall(kickPoint, direction);
        break;
    case kicking:
        skill = new Skill::Kick();
        break;
    case idling:
        skill = new Skill::Stop();
        break;
    }

    // Perform the skill
    skill->perform(robot);

    switch (state)
    {
    case driving:
        cout << "in switch driving!"<<endl;
        if (abs(rp.x - kickPoint.x) < 110 && Measurments::isClose(rp,bp,110))
            state = kicking;
        break;
    case kicking:
        cout << "in switch kicking!"<<endl;
        state = idling;
        break;
    case idling:
        cout << "in switch idling!"<<endl;
        if (!Measurments::isClose(rp, bp, 110))
            state = driving;
        break;
    }
}
