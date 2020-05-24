#ifndef MARKBOT_H
#define MARKBOT_H

#include "genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"
#include "gui/gui_interface.h"
#include "../skills/kicktopointomni.h"

/*!
 * @file
 * @author Muhaimen Shamsi
 * @brief MarkBot will position the robot between the ball and an opponent (that does not possess the ball).
 * The updateMark() function determines whether an opponent gets marked.
 */

class MarkBot : public GenericMovementBehavior
{
public:
    MarkBot(Robot*);
    void perform() override;
    bool isFinished() override;
    string getName() override;
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