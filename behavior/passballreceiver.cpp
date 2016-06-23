#include "utilities/measurements.h"
#include "behavior/passballsender.h"
#include "skill/stop.h"
#include "skill/kicktopointomni.h"
#include "skill/kick.h"
#include "utilities/region/rectangle.h"
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
    struct sides {  Rectangle up =  Rectangle(-2500, 2000, -3000, -2000);
                    Rectangle down = Rectangle(2500, 2000, 3000, -2000);
                    Rectangle left = Rectangle(-3000, 2000, 3000, 1500);
                    Rectangle right = Rectangle(-3000, -2000, 3000, -1500);
                 };
    sides fieldSides;

    Rectangle riskRegion = Rectangle (0, -2000, -3000, 2000);
    if (fieldSides.up.contains(robot->getPosition()) ||
        fieldSides.down.contains(robot->getPosition()) ||
        fieldSides.left.contains(robot->getPosition()) ||
        fieldSides.right.contains(robot->getPosition()) ||
        riskRegion.contains(robot->getPosition()))
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
    Point appGoal = gm->getOppGoal();
    Robot* oppGolie = gm->findOpTeam(GOALIE_ID);
    Point goliePos = oppGolie->getPosition();
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
            kickToPoint = new Skill::KickToPointOmni(&goalArea, -1, -1, true);
            state = kicking;
        } else {
            //Otherwise,we move to where the passer is going to kick, with a little
            //to make the passer kick in front of us
            float robGoalAng = Measurements::angleBetween(robot, goalArea);
            Point passTarget = getPasserPassPoint();
            float goal2Target = Measurements::angleBetween(goalArea, passTarget);
            Point movePoint = passTarget + Point(100 * cos(goal2Target),
                                                 100 * sin(goal2Target));
            setMovementTargets(movePoint, robGoalAng);
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
