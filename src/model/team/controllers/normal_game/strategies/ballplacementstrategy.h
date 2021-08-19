#ifndef BALLPLACEMENTSTRATEGY_H
#define BALLPLACEMENTSTRATEGY_H
#include "model/team/team_strategy.h"

class BallPlacementStrategy : public TeamStrategy
{
public:
    BallPlacementStrategy(RobotTeam* _team);

    void assignBehaviors() override;
    virtual QString getName() override;
    //int  dribble_CompletionCount;
    void runControlCycle() override;
private:
    int  dribble_CompletionCount;
};

#endif // BALLPLACEMENTSTRATEGY_H
