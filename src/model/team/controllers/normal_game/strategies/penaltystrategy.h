#ifndef PENALTYSTRATEGY_H
#define PENALTYSTRATEGY_H
#include "model/team/team_strategy.h"


/*! @file
 * Strategy to implement a penalty kick game state 'P' and 'p'
 * According to RoboCup rules, the ball is already at the
 * penalty point when this is called, so we just aim at
 * the open part of the goal-post.
 */

class Behavior;

class PenaltyStrategy:public TeamStrategy
{
public:
    PenaltyStrategy(RobotTeam* _team);
    void assignBehaviors() override;
    virtual QString getName() override;
private:
    Behavior * beh;
};

#endif // PENALTYSTRATEGY_H
