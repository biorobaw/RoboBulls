#include "simplebehaviors.h"
#include "model/gamemodel.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "utilities/point.h"
#include "include/config/team.h"
#include <stdlib.h>     //for using the function sleep

#if SIMULATED
    #define DIST 250
    #define ANGLE (7*M_PI/180)
    #define CLOSE_ENOUGH 110
#else
    #define DIST 350
    #define ANGLE (15*M_PI/180)
    #define CLOSE_ENOUGH 200
#endif

SimpleBehaviors::SimpleBehaviors()
{
    hasTargetPos = false;
}

void SimpleBehaviors::perform(Robot * r)
{
    GameModel *gm = GameModel::getModel();
    Point robotPosition = r->getRobotPosition();
    Point ballPosition = gm->getBallPoint();

    if (gm->getGameState() == 'H' || gm->getGameState() == ' '
            || gm->getGameState() == 'T' || gm->getGameState() == 't')
    {
        Skill::Stop s;
        for (unsigned i = 0; i < gm->getMyTeam().size(); i++)
        {
            s.perform(gm->getMyTeam().at(i));
        }
    }
    else if ((gm->getGameState() == 'P' || gm->getGameState() == 'p'
              || gm->getGameState() == 'F' || gm->getGameState() == 'f'
              || gm->getGameState() == 'I' || gm->getGameState() == 'i')
             && ((Measurments::distance(ballPosition, gm->getMyGoal()) < 1200)
                 || (Measurments::distance(ballPosition, gm->getOpponentGoal()) < 1200)))
    {
        Skill::Stop s;
        for (unsigned i = 0; i < gm->getMyTeam().size(); i++)
        {
            s.perform(gm->getMyTeam().at(i));
        }
    }
    else if ((gm->getGameState() == 'P' || gm->getGameState() == 'p'
             || gm->getGameState() == 'F' || gm->getGameState() == 'f'
             || gm->getGameState() == 'I' || gm->getGameState() == 'i')
        && ((Measurments::distance(ballPosition, gm->getMyGoal()) >= 1200)
            || (Measurments::distance(ballPosition, gm->getOpponentGoal()) >= 1200)))
    {
        #if SIMULATED
            #define DISTANCE 350
        #else
            #define DISTANCE 800
        #endif
        Point goal;
        if (((gm->getGameState() == 'P' || gm->getGameState() == 'F' || gm->getGameState() == 'I') && TEAM == TEAM_BLUE)
                ||((gm->getGameState() == 'p' || gm->getGameState() == 'f' || gm->getGameState() == 'i') && TEAM == TEAM_YELLOW))
            goal = gm->getOpponentGoal();
        else if (((gm->getGameState() == 'p' || gm->getGameState() == 'f' || gm->getGameState() == 'i') && TEAM == TEAM_BLUE)
                 || ((gm->getGameState() == 'P' || gm->getGameState() == 'F' || gm->getGameState() == 'I') && TEAM == TEAM_YELLOW))
            goal = gm->getMyGoal();
//        float targetBallAngle = Measurments::angleBetween(goal, ballPosition);
        float px;
        if (ballPosition.x > 0)
            px = -1500;
        else
            px = 1500;

        if(!hasTargetPos) {
            hasTargetPos = true;
            target.y = robotPosition.y;
        }
        target.x = px;

        float direction = Measurments::angleBetween(robotPosition, ballPosition);

        move.recreate(target, direction, true);
        move.perform(r);
    }
}
