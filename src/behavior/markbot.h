#ifndef MARKBOT_H
#define MARKBOT_H

#include "genericmovementbehavior.h"
#include "src/model/game_state.h"
#include "src/utilities/region/defencearea.h"
#include "src/utilities/comparisons.h"
#include "src/gui/guiinterface.h"
#include "src/skill/kicktopointomni.h"

/*!
 * @file
 * @author Muhaimen Shamsi
 * @brief MarkBot will position the robot between the ball and an opponent (that does not possess the ball).
 * The updateMark() function determines whether an opponent gets marked.
 */

class MarkBot : public GenericMovementBehavior
{
public:
    MarkBot();
    void perform(Robot *) override;
    bool isFinished() override;

private:
    // Variables for tracking marked/markable opponents
    static bool mark_status[10];
    int marked_opp_id = -1;

    /*! Manages which robots are marked by all robots assigned this behavior.
     * Updates a static array to indicate which robots are marked.
     * This allows this behavior to be assigned to multiple robots without
     * resulting in reduntant markings.
     * Updates \c marked_opp_id with the ID of the opponent to mark.
     */
    void updateMark(Robot*);
};

#endif // MARKBOT_H
