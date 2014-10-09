#ifndef KICKOFFSTRATEGY_H
#define KICKOFFSTRATEGY_H

#include "strategy/strategy.h"
#include "movement/gotoposition.h"
#include "model/gamemodel.h"


class KickOffStrategy:public Strategy
{
public:
    KickOffStrategy();
    void assignBeh();
    bool update() override;
private:
    Movement::GoToPosition move_skill0,move_skill1,move_skill2,move_skill3,move_skill4,move_skill5;
    Point move_point0,move_point1,move_point2,move_point3,move_point4,move_point5, bp;
    float orientation0,orientation1,orientation2,orientation3,orientation4,orientation5;
    GameModel * gm;
};

#endif // KICKOFFSTRATEGY_H
