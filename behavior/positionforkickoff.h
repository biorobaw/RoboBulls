#ifndef POSITIONFORKICKOFF_H
#define POSITIONFORKICKOFF_H

#include "behavior.h"
#include "skill/obstacleavoidmove.h"
#include "skill/gotopositionwithorientation.h"
#include "model/gamemodel.h"
#include "utilities/paramlist.h"

/* Sends Robot 5 to goalpost
   Sends Robot 4, 3, and 2 to a defensive line
   Sends Robot 1, 0 to center field
*/

class PositionForKickoff:public Behavior
{
private:
    Skill::Skill * move_skill;
    Point move_point, bp;
    bool bp_updated;
    float orientation;
    GameModel * gm;

public:
    PositionForKickoff(const ParameterList &list);
    void perform(Robot *);
};

#endif // POSITIONFORKICKOFF_H
