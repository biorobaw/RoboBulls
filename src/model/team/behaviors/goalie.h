#ifndef Goalie_H
#define Goalie_H
#include <math.h>
#include "../skills/kicktopointomni.h"
#include "../skills/dribbleback.h"
#include "genericmovementbehavior.h"
#include "utilities/region/defencearea.h"
#include "utilities/measurements.h"
#include "utilities/comparisons.h"
#include "gui/interface/gui_interface.h"


/*! @file
 * Behavior of goal-keeper.
 *
 * The robot defends the goal by positioning itself in the trajectory of the ball
 * when it is moving torwards the goal, as well as positioning itself to block a robot
 * when it has the ball and is facing the goal.
 * When the ball becomes close, the goalie moves out and kicks the ball to the
 * center of the field to remove it from the goalie box.
 */

class Goalie:public GenericMovementBehavior
{
public:
    Goalie(Robot*);
    ~Goalie();
    bool perform() override;
    string getName() override;

private:
    /*! Returns true if the ball is moving with non-zero velocity torwards the goal.
     * This is used to move the goalie in the path of a ball moving to the goal
     * @param lineEndsOut If true, A pair of Point of {the ball's position, where it will land in the goal} */
    bool isBallMovingTowardsGoal(std::pair<Point,Point>& lineEndsOut);

    /*! Returns true if `robot` is facing our goal.
     * This is used to place the goalie in the path of a possible robot kick.
     * @param robot Robot to test
     * @param lineSegOut If true, A pair of Point of {the ball's position, where it will land in the goal} */
    bool botOnBallIsAimedAtOurGoal(Robot* robot, std::pair<Point,Point>& lineSegOut);

    /*! Returns true if the ball is either behind the goal or too close to the edge
     *  In such cases, we will not chase the ball as it is out of the field or unreachable */
    bool isBallReachable();

    /*! Returns true if the ball is stopped in the defence area
     *  We will only try to kick the ball away once this returns true */
    bool shouldClearBall();

    /*! Returns true if the ball is stopped just outside the defence area
     *  We will only try to kick the ball away once this returns true */
    bool shouldRetrieveBall(Robot* robot);

    Point goalPoint;                        //!< Center of our team goal area
    Point idlePoint;                        //!< Point to sit at when no action happens
    Point kickPoint;                        //!< Point to kick the point to

    KickToPointOmni* kick_skill;     //!< Skill to kick the ball with
    DribbleBack* dribble_skill;      //!< Skill to kick the ball with

    bool retrieving_ball = false;           //!< Indicates that the ball is being retrieved
    bool clearing_ball = false;             //!< Indicates that the ball is being retrieved
    DefenceArea def_area;
};

#endif // Goalie_H
