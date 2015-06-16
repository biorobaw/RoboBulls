#include "behavior/genericmovementbehavior.h"
#include "behavior/defendfarfromball.h"
#include "include/config/team.h"
#include "model/gamemodel.h"
#include "kickoffstrategy.h"

/* For this, we define a list of points with absolute Xs representing
 * where the robots should go on Blue Kickoff / Yellow Kickoff;
 * there's diagrams commented out below. These are multiplied by
 * GameModel::mySide to keep them on the right side. Points are not
 * assigned based on ID, but in the order robots appear in the team vector.
 */

/******************
 *             *
 *    4        *
 *       2    |-|
 | 5        0 |o|
 *       1    |-|
 *    3        *
 *             *
 ******************/
Point KickOffStrategy::myKickoffPoints[10] = {
    /*0*/ Point( 200,    0),
    /*1*/ Point( 800, -600),
    /*2*/ Point( 800,  600),
    /*3*/ Point(1300, -900),
    /*4*/ Point(1300,  900),
    /*5*/ Point(2900,    0)
};

/******************
 *             *
 *   1  3      *
 *            |-|
 | 5    0     |o|
 *            |-|
 *   2  4      *
 *             *
 ******************/
Point KickOffStrategy::opKickoffPoints[10] = {
    /*0*/ Point(2000,    0),
    /*1*/ Point(2200,  800),
    /*2*/ Point(2200, -800),
    /*3*/ Point(2000,  800),
    /*4*/ Point(2000, -800),
    /*5*/ Point(2900,    0)
};


void KickOffStrategy::assignBeh()
{
    char gs = gameModel->getGameState();
    Point* whichKickoffPointList = myKickoffPoints;
    
    //Change to other point list if it's not our kickoff
    if ((gs=='K' && TEAM==TEAM_YELLOW) || (gs=='k' && TEAM==TEAM_BLUE ))
        whichKickoffPointList = opKickoffPoints;
    
    int i = 0;
    for(Robot* rob : gameModel->getMyTeam()) 
	{
        if(rob->getID() == 5)
            continue;
        Point nextPoint = whichKickoffPointList[i++];
        nextPoint.x *= GameModel::mySide;   //Keeps X values on correct side
        float angleToBall = Measurments::angleBetween(nextPoint, gameModel->getBallPoint());
        rob->assignBeh<GenericMovementBehavior>(nextPoint, angleToBall);
    }

    //Goalie is a special case
    Robot* goalie = gameModel->findMyTeam(5);
    if(goalie)
        goalie->assignBeh<DefendFarFromBall>();
}
