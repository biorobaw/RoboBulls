#ifndef ATTACK_SUPPORT_H
#define ATTACK_SUPPORT_H
#include "behavior/genericmovementbehavior.h"
#include "utilities/region.h"
#include "model/gamemodel.h"
#include "model/robot.h"

/*! @brief AttackSupport is a complement to Attackmain, used in NormalGameStrategy.
 * @author Muhaimen Shamsi, JamesW
 * @details It goes to the most optimal half of the enemy field--the one with least
 * enemies, and the one that the AttackMain robot is not inside of--and waits
 * for a pass from the AttackMain robot. */

class AttackSupport : public GenericMovementBehavior
{
public:
    AttackSupport(Robot* passer);
    void perform(Robot *);
    Point getCurrentTarget();

private:
    Robot* main_attacker;       //Pointer to robot passing to us
    void recalculateWp(Robot*); //Re-calculates wp be to the "best" area
    Point wp;                   //Point to wait at for a pass
    Point previousAttackerPos;  //Previous attacker position, used to not reclaculate wp so often
};

#endif // ATTACK_SUPPORT_H
