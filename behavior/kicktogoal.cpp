#include <algorithm>
#include "kicktogoal.h"
#include "include/config/simulated.h"
#include "model/gamemodel.h"
#include "skill/stop.h"
#include "skill/kicktopointomni.h"

#if SIMULATED
    #define ANGLE   (10 * M_PI/180)
    #define CLOSE_TO_BALL 110
    #define BEHIND_RADIUS (ROBOT_RADIUS*2.25)
    #define CLOSE_ENOUGH 110
    #define R   200
#else
    #define ANGLE   (5 * M_PI/180)
    #define CLOSE_TO_BALL 110
    #define BEHIND_RADIUS (ROBOT_RADIUS*2.25)
    #define CLOSE_ENOUGH 220
    #define R   100
#endif

#define KTGOAL_DEBUG 0

KickToGoal::KickToGoal()
{
    ballOrig = GameModel::getModel()->getBallPoint();
    state = initial;
    okay = false;
    delayCount = 0;
}

KickToGoal::~KickToGoal()
{
    delete kickToPoint;
}

bool KickToGoal::checkifOkay()
{
    if(++delayCount < 100)
        return false;
    const auto& myT = gameModel->getMyTeam();
    return std::all_of(myT.begin(),myT.end(),[](Robot* r){return r->getSpeed() < 0.15;});
}

void KickToGoal::perform(Robot * r)
{
    GameModel* gm = GameModel::getModel();
    Point appGoal = gm->getOpponentGoal();
    Robot* oppGolie = gm->findOpTeam(5);
    Point goaliePos = oppGolie->getRobotPosition();
    float min_y = appGoal.y - R;
    float max_y = appGoal.y + R;
    Point goalArea;
    goalArea.x = appGoal.x;

    //Waits until all robots have moved away before doings things
    if(!okay) {
        okay = checkifOkay();
        if(!okay) return;
    }

    goalArea.y = (goaliePos.y >= 0) ? min_y : max_y;

    // Evaluate possible transitions
    switch(state)
    {
    case initial:
        kickToPoint = new Skill::KickToPointOmni(goalArea, ANGLE);
        state = kicking;
        break;

    case kicking:
        {
        #if KTGOAL_DEBUG
            cout<<"kicking performed!"<<endl;
        #endif
            bool kicked = kickToPoint->perform(r);
            if (kicked) {
                state = stopping;
            }
        }
        break;

    case stopping:
        {
        #if KTGOAL_DEBUG
            cout<<"idling performed!"<<endl;
        #endif
            Skill::Stop stop;
            stop.perform(r);
        }
        break;
    }
}

bool KickToGoal::isFinished()
{
    return state == stopping;
}
