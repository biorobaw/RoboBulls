#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H
#include "skill/kicktopointomni.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

/*! @brief The “goalie” behavior of the game.
 * @author Narges Ghaedi, James W
 *
 * The robot defends the goal by positioning itself in the trajectory of the ball
 * when it is moving torwards the goal, as well as positioning itself to block a robot
 * when it has the ball and is facing the goal.
 *
 * When the ball becomes close, the goalie moves out and kicks the ball to the
 * center of the field to remove it from the goalie box.
 */

class DefendFarFromBall : public GenericMovementBehavior
{
public:
    static int goalieDist; //!< @brief Distance to goal needed for goalie to react

public:
    DefendFarFromBall();
    void perform(Robot*);

private:
    /*! @brief Returns true if the ball is moving with non-zero velocity torwards the goal.
     * @param lineEndsOut If true, A pair of Point of {the ball's position, where it will land in the goal} */
    bool isBallMovingTowardsGoal(std::pair<Point,Point>& lineEndsOut);

    Point idlePoint; //!< Point to sit at when no action happens
};

#endif // DEFENDFARFROMBALL_H
