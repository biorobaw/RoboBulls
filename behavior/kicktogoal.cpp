#include "kicktogoal.h"

#include "model/gamemodel.h"
#include "skill/skill.h"
#include "skill/kick.h"
#include "skill/stop.h"

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
    #define R   200
#endif

KickToGoal::KickToGoal()
    : GenericMovementBehavior()
{
    ballOrig = GameModel::getModel()->getBallPoint();
    state = initial;
    sign = 0;
    targetSign = 0;
}

void KickToGoal::perform(Robot * r)
{
    GameModel* gm = GameModel::getModel();
    Point ballPos = gm->getBallPoint();
    Point appGoal = gm->getOpponentGoal();
    Robot* oppGolie = gm->findOpTeam(5);
    Point goaliePos = oppGolie->getRobotPosition();
    float min_y = appGoal.y - R;
    float max_y = appGoal.y + R;
    Point goalArea;
    goalArea.x = appGoal.x;
    bool ballInFront =
            abs(Measurments::angleDiff(r->getOrientation(), Measurments::angleBetween(r->getRobotPosition(), ballPos))) < ANGLE*3;
    bool kicked = !Measurments::isClose(ballPos, ballOrig) && !ballInFront;

    if (goaliePos.y >= 0)
    {
        goalArea.y = min_y;
    }
    else
    {
        goalArea.y = max_y;
    }
    cout << "goali at\t" << goaliePos.toString() << "\tkick to\t" << goalArea.toString()<< endl;
    // Evaluate possible transitions
    switch(state)
    {
    case initial:
        cout << "initial" << endl;
        kickToPoint = new Skill::KickToPoint(goalArea, ANGLE);
        state = kicking;
        if (goaliePos.y >= 0)
            sign = 0;
        else
            sign = 1;

        targetSign = sign;
        break;
    case kicking:
        cout << "kicking" << endl;
        {
        if (goaliePos.y >= 0)
            sign = 0;
        else
            sign = 1;
           kickToPoint->perform(r);
        #if PENALTY_BEHAVIOR_DEBUG
            cout<<"kicking performed!"<<endl;
        #endif
        if (sign != targetSign)
        {
            state = initial;
            targetSign = sign;
        }
        else if (kicked)
            state = stopping;

        }

        break;
    case stopping:
        cout << "idling" << endl;
        {
            Skill::Stop stop;
            stop.perform(r);
        #if PENALTY_BEHAVIOR_DEBUG
            cout<<"idling performed!"<<endl;
        #endif
        }
        break;
    }
}
