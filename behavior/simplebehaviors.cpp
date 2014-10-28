#include "simplebehaviors.h"
#include "model/gamemodel.h"
#include "skill/stop.h"
#include "skill/kicktopoint.h"
#include "utilities/point.h"
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

SimpleBehaviors::SimpleBehaviors(const ParameterList& list)
{
    UNUSED_PARAM(list);
}

void SimpleBehaviors::perform(Robot * r)
{
    GameModel *gm = GameModel::getModel();
    Point robotPosition = r->getRobotPosition();
    Point ballPosition = gm->getBallPoint();

    if (gm->getGameState() == 'H')
    {
        Skill::Stop s;
        for (unsigned i = 0; i < gm->getMyTeam().size(); i++)
        {
            s.perform(gm->getMyTeam().at(i));
        }
    }
    else if (gm->getGameState() == 'P')
    {
        #if SIMULATED
            #define DISTANCE 250
        #else
            #define DISTANCE 500
        #endif

        Point position(gm->getPenaltyPoint().x-DISTANCE, robotPosition.y);
        float direction = Measurments::angleBetween(robotPosition, ballPosition);

        move.setMovementTolerances(CLOSE_ENOUGH, ANGLE);
        move.setVelocityMultiplier(1);
        move.recreate(position, direction, false);
        move.perform(r);
    }
    else if (gm->getGameState() == 'F')
    {
        #if SIMULATED
            #define DISTANCE 250
        #else
            #define DISTANCE 500
        #endif

        Point position(gm->getBallPoint().x-DISTANCE, robotPosition.y);
        float direction = Measurments::angleBetween(robotPosition, ballPosition);

        move.setMovementTolerances(CLOSE_ENOUGH, ANGLE);
        move.setVelocityMultiplier(1);
        move.recreate(position, direction, false);
        move.perform(r);
    }

}
