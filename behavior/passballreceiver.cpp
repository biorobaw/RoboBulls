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

#define PASSBALRECV_DEBUG 0

PassBallReceiver::PassBallReceiver(Robot* theSender)
{
    passer = theSender;
    state = initial;
    kickToPoint = nullptr;
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

Point PassBallReceiver::getPasserPassPoint()
{
    /* Here we downcast the generic Behavior pointer to use a method specific
     * to PassBallSender
     */
    PassBallSender* s = dynamic_cast<PassBallSender*>(passer->getCurrentBeh());
    return s->getPassingPoint();
}

void PassBallReceiver::perform(Robot *robot)
{
    GameModel *gm = GameModel::getModel();
    Point appGoal = gm->getOpponentGoal();
    Robot* oppGolie = gm->findOpTeam(5);
    Point goliePos = oppGolie->getRobotPosition();
    float min_y = appGoal.y - R;
    float max_y = appGoal.y + R;

    goalArea.x = appGoal.x;

    if (goliePos.y >= 0) {
        goalArea.y = min_y;
    } else {
        goalArea.y = max_y;
    }

    switch(state)
    {
    case initial:
        if(passer->getCurrentBeh()->isFinished()) {
            //If the passer kicked, we move to kick the ball to the goal
            kickToPoint = new Skill::KickToPointOmni(&goalArea);
            state = kicking;
        } else {
            //Otherwise,we move to where the passer is going to kick.
            float a = Measurments::angleBetween(robot, passer);
            setMovementTargets(getPasserPassPoint(), a);
            GenericMovementBehavior::perform(robot);
        }
        break;

    case kicking:
        if(kickToPoint->perform(robot)) {
            state = idling;
        }
        break;

    case idling:
        {
            Skill::Stop stop;
            stop.perform(robot);
        }
        break;
    }
}

bool PassBallReceiver::isFinished()
{
    return state == idling;
}
