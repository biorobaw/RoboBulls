#ifndef PATHPLANNER_H
#define PATHPLANNER_H

#include <deque>
#include "utilities/point.h"


class Robot;
class GameState;
class Ball;

/*!
 * \brief Abstract Path Planner class
 */
class PathPlanner
{
public:
    typedef std::deque<Point> Path;

    PathPlanner(Robot* self);
    virtual ~PathPlanner();

    /*! @brief Abstract function to find a path from Point `start` to Point `end`.
     * @details Returns a deque of waypoints indicating the path.
     * @param goal The ending point of the path
     * @param avoidBall Controls counting the ball as an obstacle or not */
    virtual Path genPath(Point goal, bool avoid_ball = true) = 0;

protected:
    Robot* self;
    GameState* game_state;
    Ball* ball;
};

#endif // PATHPLANNER_H
