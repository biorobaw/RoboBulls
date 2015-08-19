#ifndef DRIVEBALL_H
#define DRIVEBALL_H
#include "skill/skill.h"
#include "utilities/point.h"
#include "model/robot.h"
#include "movement/gotoposition.h"

namespace Skill
{

/*! @brief Pushes (drives) the ball a certain distance
 * @author Narges Ghaedi, James Waugh
 * @details Attempts to push the ball to the point targetPoint
 * at angle finalDirection
 * @deprecated Use KickToPointOmni with a kick distance instead */

class DriveBall:public Skill
{
public:
    DriveBall(Point targetPoint, double finalDirection);
    bool perform(Robot*) override;
private:
    Point  targetPosition;
    double direction;
    enum states {moveBehindBall, driveBall} state;
    Movement::GoToPosition move_skill;
};

}

#endif // DRIVEBALL_H
