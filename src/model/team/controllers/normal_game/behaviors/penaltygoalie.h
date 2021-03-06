#ifndef PENALTYGOALIE_H
#define PENALTYGOALIE_H

#include "model/robot/behaviors/genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "gui/interface/gui_interface.h"

/*!
 * @file
 * @author Muhaimen Shamsi
 * @brief Behavior for the goalie to defend against penalty shots.
 *
 * The is designed to be compliant with the penatly rules for SSL:
 *
 * - The goalie remains between the goalposts
 * - touches its goal line
 * - and faces outward of the goal
 *
 * These rules must be satisfied until the ball has been kicked.
 * It is allowed to move before the ball has been kicked, as long
 * as itsmotion does not break any of these constraints.
 */

class PenaltyGoalie : public GenericMovementBehavior
{
public:
    PenaltyGoalie(Robot*);
    bool perform() override;
    bool isFinished() override;
    string getName() override;
private:

};

#endif // PENALTYGOALIE_H
