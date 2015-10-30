#ifndef DEFENDFARFROMBALL_H
#define DEFENDFARFROMBALL_H
#include "skill/kicktopointomni.h"
#include "behavior/genericmovementbehavior.h"
#include "movement/gotoposition.h"

/*! @brief The “goalie” behavior of the game.
 * @author Narges Ghaedi, James W, Adam H
 *
 * The robot defends the goal by positioning itself in the trajectory of the ball
 * when it is moving torwards the goal, as well as positioning itself to block a robot
 * when it has the ball and is facing the goal.
 * When the ball becomes close, the goalie moves out and kicks the ball to the
 * center of the field to remove it from the goalie box.
 */

class DefendFarFromBall : public GenericMovementBehavior
{
public:
    static int goalieDist; //!< Distance ball must be to idlePoint for goalie to move it kick it

public:
    DefendFarFromBall();
    ~DefendFarFromBall();
    void perform(Robot*);

private:
    /*! @brief Returns true if the ball is moving with non-zero velocity torwards the goal.
     * @details This is used to move the goalie in the path of a ball moving to the goal
     * @param lineEndsOut If true, A pair of Point of {the ball's position, where it will land in the goal} */
    bool isBallMovingTowardsGoal(std::pair<Point,Point>& lineEndsOut);

    /*! @brief Returns true if `robot` is facing our goal.
     * @details This is used to place the goalie in the path of a possible robot kick.
     * @param robot Robot to test
     * @param lineSegOut If true, A pair of Point of {the ball's position, where it will land in the goal} */
    bool ballOnRobotIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut);

    /*! @brief Returns true if the ball is either behind the goal or too close to the edge
     * @details In such cases, we will not chase the ball as it is out of the field or unreachable */
    bool isBallUnreachable();

    Point idlePoint;                    //!< Point to sit at when no action happens
    bool isKickingBallAway;             //!< Are we in the process of kicking the ball from the goal?
    bool isIdling;                      //!< Are we idling at the idle point?
    Skill::KickToPointOmni* kick_skill; //!< Skill to kick the ball with
};

#endif // DEFENDFARFROMBALL_H
