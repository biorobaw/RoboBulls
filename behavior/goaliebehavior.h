#ifndef GOALIEBEHAVIOR_H
#define GOALIEBEHAVIOR_H
#include <math.h>
#include "skill/kicktopointomni.h"
#include "skill/dribbleback.h"
#include "behavior/genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "gui/guiinterface.h"
#include "include/config/team.h"

/*! @brief The “goalie” behavior of the game.
 * @author Narges G, James W, Adam H, Muhaimen S
 *
 * The robot defends the goal by positioning itself in the trajectory of the ball
 * when it is moving torwards the goal, as well as positioning itself to block a robot
 * when it has the ball and is facing the goal.
 * When the ball becomes close, the goalie moves out and kicks the ball to the
 * center of the field to remove it from the goalie box.
 */

class GoalieBehavior:public GenericMovementBehavior
{
public:
    GoalieBehavior();
    ~GoalieBehavior();
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
    bool botOnBallIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut);

    /*! @brief Returns true if the ball is either behind the goal or too close to the edge
     * @details In such cases, we will not chase the ball as it is out of the field or unreachable */
    bool isBallReachable();

    /*! @brief Returns true if the ball is stopped in the defence area
     * @details We will only try to kick the ball away once this returns true */
    bool shouldClearBall();

    /*! @brief Returns true if the ball is stopped just outside the defence area
     * @details We will only try to kick the ball away once this returns true */
    bool shouldRetrieveBall();

    Point idlePoint;                        //!< Point to sit at when no action happens
    Point kickPoint;                        //!< Point to kick the point to
    Skill::KickToPointOmni* kick_skill;     //!< Skill to kick the ball with
    Skill::DribbleBack* dribble_skill;   //!< Skill to kick the ball with
    bool retrieving_ball = false;           //!< Indicates that the ball is being retrieved
    DefenceArea def_area;
};

#endif // GoalieBehavior_H
