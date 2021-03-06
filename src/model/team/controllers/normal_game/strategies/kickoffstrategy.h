#ifndef KICKOFFSTRATEGY_H
#define KICKOFFSTRATEGY_H
#include "model/team/team_strategy.h"
#include "utilities/point.h"

/*! @file
 *  Strategy to implement a kickoff game state.
 *  The KickOffStrategy moves the robots in a static formation.
 *  There are different configs for both enemy and our kickoffs.
 */

class KickOffStrategy : public TeamStrategy
{
public:
    KickOffStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    virtual QString getName() override;
private:
#define NUM_KICK_OFF_POINTS 5
    static Point myKickoffPoints[NUM_KICK_OFF_POINTS];
    static Point opKickoffPoints[NUM_KICK_OFF_POINTS];
};

#endif // KICKOFFSTRATEGY_H
