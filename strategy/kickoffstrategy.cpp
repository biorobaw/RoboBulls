#include <list>
#include "behavior/genericmovementbehavior.h"
#include "behavior/goaliebehavior.h"
#include "include/config/team.h"
#include "model/gamemodel.h"
#include "utilities/comparisons.h"
#include "kickoffstrategy.h"

/* For this, we define a list of points with absolute Xs representing
 * where the robots should go on Blue Kickoff / Yellow Kickoff;
 * there's diagrams commented out below. These are multiplied by
 * GameModel::mySide to keep them on the right side. Points are not
 * assigned based on ID, but each robot claims the next closest point
 * to it.
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
Point KickOffStrategy::myKickoffPoints[6] = {
    /*0*/ Point( -200,    0),
    /*1*/ Point( -800, -600),
    /*2*/ Point( -800,  600),
    /*3*/ Point(-1300, -900),
    /*4*/ Point(-1300,  900),
    /*5*/ Point(-2900,    0)
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
Point KickOffStrategy::opKickoffPoints[6] = {
    /*0*/ Point(-1200,    0),
    /*1*/ Point(-1900,  800),
    /*2*/ Point(-1900, -800),
    /*3*/ Point(-1200,  800),
    /*4*/ Point(-1200, -800),
    /*5*/ Point(-2900,    0)
};

void KickOffStrategy::assignBeh()
{
    char gs = gameModel->getGameState();
    Point* whichKickoffPointList = myKickoffPoints;
    
    //Change to other point list if it's not our kickoff
    if ((gs=='K' && OUR_TEAM==TEAM_YELLOW) || (gs=='k' && OUR_TEAM==TEAM_BLUE ))
        whichKickoffPointList = opKickoffPoints;

    //List of points for fast removal
    std::list<Point> pointList(whichKickoffPointList, whichKickoffPointList+6);

    for(Robot* robot : gameModel->getMyTeam())
	{
        if(robot->getID() == GOALIE_ID)
            continue;

        //We look for the closest point as the robots come, then remove it
        auto nextPointItr = Comparisons::distance(robot).min(pointList);
        Point nextPoint = *nextPointItr;
        pointList.erase(nextPointItr);

        //Assign to move to that point, then face to the center
        float angleToCenter = Measurements::angleBetween(nextPoint, Point(0,0));
        robot->assignBeh<StaticMovementBehavior>(nextPoint, angleToCenter);
    }

    //Goalie is a special case
    Robot* goalie = gameModel->findMyTeam(GOALIE_ID);
    if(goalie)
        goalie->assignBeh<GoalieBehavior>();
}
