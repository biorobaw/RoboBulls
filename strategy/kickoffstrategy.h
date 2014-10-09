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
    GameModel * gm;
    Point bp;
};

#endif // KICKOFFSTRATEGY_H
