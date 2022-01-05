#ifndef DUMMY_H
#define DUMMY_H

#include "model/robot/behavior.h"
#include "model/robot/behaviors/skills/kicktopointomni.h"
#include "model/robot/behaviors/skills/dribbletopoint.h"
#include "gui/interface/gui_interface.h"
#include "utilities/region/sector.h"
#include "model/robot/behaviors/genericmovementbehavior.h"
#include "algorithm"
#include "utilities/region/defencearea.h"
#include "utilities/comparisons.h"


#include <vector>


/*! @file
 * dummy file for gathering Neural Network Data
 * Strafes vertically while maintaining horizontal position.
 */


class Dummy : public Behavior//:public GenericMovementBehavior
{
public:
    Dummy(Robot*);
   ~Dummy();
    bool perform() override;
    bool isFinished() override;

    string getName() override;

private:
    int sign = 1;
    bool finished = false;
    Point Target;

};

#endif // DUMMY_H
