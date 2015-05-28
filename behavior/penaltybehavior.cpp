#include <iostream>
#include "skill/stop.h"
#include "skill/kicktopointomni.h"
#include "penaltybehavior.h"

#if SIMULATED
    #define DIST 250
    #define ANGLE (7*M_PI/180)
    #define CLOSE_ENOUGH 110
    #define R   200
#else
    #define DIST 270
    #define ANGLE (7*M_PI/180)
    #define CLOSE_ENOUGH 170
    #define R   200
#endif

PenaltyBehavior::PenaltyBehavior()
{
    pb = initial;
    sign = 0;
    targetSign = 0;
    ballOrig = GameModel::getModel()->getBallPoint();
}

PenaltyBehavior::~PenaltyBehavior()
{
    delete kickToPoint;
}

/**
 * @brief PenaltyBehavior::perform Behavior that goes to the penalty point and kicks the ball once.
 * @param myRobot : The robot to perform the behavior on (usually myTeam[0])
 */
void PenaltyBehavior::perform(Robot * myRobot)
{
    GameModel *model = GameModel::getModel();
    Point appGoal = model->getOpponentGoal();
    Robot* oppGolie = model->findOpTeam(5);

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

    switch(pb)
    {
    case initial:
        cout << "initial" << endl;
        kickToPoint = new Skill::KickToPointOmni(goalArea);
        pb = kicking;
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
           bool kicked = kickToPoint->perform(myRobot);
        #if PENALTY_BEHAVIOR_DEBUG
            cout<<"kicking performed!"<<endl;
        #endif
//            pb = idling;
        if (sign != targetSign)
        {
            pb = initial;
            targetSign = sign;
        }
        else if (kicked)
            pb = idling;
        }

        break;
    case idling:
        cout << "idling" << endl;
        {
            Skill::Stop stop;
            stop.perform(myRobot);
        #if PENALTY_BEHAVIOR_DEBUG
            cout<<"idling performed!"<<endl;
        #endif
        }
        break;
    }
}
