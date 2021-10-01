#ifndef BALLPLACEMENTSTRATEGY_H
#define BALLPLACEMENTSTRATEGY_H
#include "model/team/team_strategy.h"
#include <QMutex>

class BallPlacementStrategy : public TeamStrategy
{
public:
    BallPlacementStrategy(RobotTeam* _team);
    ~BallPlacementStrategy();
    void assignBehaviors() override;
    virtual QString getName() override;
    //int  dribble_CompletionCount;
    void runControlCycle() override;
private:
    int  dribble_CompletionCount;
    enum states{OUTSIDE_FIELD, INSIDE_FIELD};
    states cur_state;
    int has_ball_ct;
    //QMutex mutex;
};

#endif // BALLPLACEMENTSTRATEGY_H
