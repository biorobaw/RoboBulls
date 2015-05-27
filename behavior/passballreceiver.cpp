#include "utilities/measurments.h"
#include "behavior/passballsender.h"
#include "skill/stop.h"
#include "skill/kicktopointomni.h"
#include "skill/kick.h"
#include "utilities/region.h"
#include "include/config/team.h"
#include "passballreceiver.h"

#if SIMULATED
    #define ANGLE   (10 * M_PI/180)
    #define CLOSE_ENOUGH 110
    #define DIST   50
    #define R   200
#else
    #define ANGLE   (5 * M_PI/180)
    #define CLOSE_ENOUGH 210
    #define DIST 400
    #define R   200
#endif

GameModel *gm = GameModel::getModel();

PassBallReceiver::PassBallReceiver()
{
    state = initial;
    ballLastSeen = gm->getBallPoint();
    count = 170;
    ballOrg = gm->getBallPoint();
    sign = 0;
    targetSign = 0;
}

PassBallReceiver::~PassBallReceiver()
{
    delete kickToPoint;
}

bool PassBallReceiver::playerInBadArea(Robot *robot)
{
    struct sides {  Region up;
                    Region down;
                    Region left;
                    Region right;
                 };
    sides fieldSides;
    fieldSides.up = Region(-2500, -3000, 2000, -2000);
    fieldSides.down = Region(2500, 3000, 2000, -2000);
    fieldSides.left = Region(-3000, 3000, 2000, 1500);
    fieldSides.right = Region(-3000, 3000, -2000, -1500);

    Region riskRegion;
    if (TEAM == TEAM_BLUE)
        riskRegion = Region (0, -3000, -2000, 2000);
    else
        riskRegion = Region (0, 3000, -2000, 2000);

    if (fieldSides.up.contains(robot->getRobotPosition()) ||
        fieldSides.down.contains(robot->getRobotPosition()) ||
        fieldSides.left.contains(robot->getRobotPosition()) ||
        fieldSides.right.contains(robot->getRobotPosition()) ||
        riskRegion.contains(robot->getRobotPosition()))
        return true;
    else
        return false;

}

void PassBallReceiver::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();

    Point ballPos = gm->getBallPoint();
    Point robotPos = robot->getRobotPosition();

    Point appGoal = gm->getOpponentGoal();
    Robot* oppGolie = gm->findOpTeam(5);
    Point goliePos = oppGolie->getRobotPosition();
    float min_y = appGoal.y - R;
    float max_y = appGoal.y + R;
    Point goalArea;
    goalArea.x = appGoal.x;

    if (goliePos.y >= 0)
    {
        goalArea.y = min_y;
    }
    else
    {
        goalArea.y = max_y;
    }

    if (Measurments::isClose(ballPos, robotPos, CLOSE_ENOUGH*2) || count == 0)
    {
        switch(state)
        {
        case initial:
            cout << "initial" << endl;
            kickToPoint = new Skill::KickToPointOmni(goalArea);
            state = kicking;
            if (goliePos.y >= 0)
                sign = 0;
            else
                sign = 1;

            targetSign = sign;
            break;
        case kicking:
            cout << "kicking" << endl;
            {
            if (goliePos.y >= 0)
                sign = 0;
            else
                sign = 1;
               kickToPoint->perform(robot);
            #if PENALTY_BEHAVIOR_DEBUG
                cout<<"kicking performed!"<<endl;
            #endif
    //            pb = idling;
            if (sign != targetSign)
            {
                state = initial;
                targetSign = sign;
            }
            }

            break;
        case idling:
            cout << "idling" << endl;
            {
                Skill::Stop stop;
                stop.perform(robot);
            #if PENALTY_BEHAVIOR_DEBUG
                cout<<"idling performed!"<<endl;
            #endif
            }
            break;
        }
    } else {
        Skill::Stop s;
        s.perform(robot);
    }

    if (Measurments::distance(ballOrg, ballPos) > CLOSE_ENOUGH && count != 0)
    {
        if (playerInBadArea(robot))
            count = 0;
        else
            count--;
    }
    cout << "count\t" << count << endl;
}
