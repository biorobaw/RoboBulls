#ifndef TWO_V_ONE_H
#define TWO_V_ONE_H

#include <algorithm>

#include "strategy/strategy.h"
#include "model/gamemodel.h"
#include "behavior/behaviorassignment.h"
#include "behavior/attackmain.h"
#include "behavior/attacksupport.h"
#include "behavior/positionforkickoff.h"
#include "behavior/defendclosetoball.h"
#include "behavior/defendfarfromball.h"
#include "behavior/sendballtoregion.h"
#include "utilities/measurments.h"


class TwoVOne:public Strategy
{
private:
    GameModel * gm;
    Point bp,mgp,ogp,r0,r1,r2,r3,r4,r5;

    vector <Region *> strategicRegions;
public:
    TwoVOne();
    void assignStrategicValue(Region *);
    void assignBeh();
};

#endif // TWO_V_ONE_H
