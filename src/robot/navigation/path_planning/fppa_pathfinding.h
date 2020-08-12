#ifndef FPPA_PATHFINDING_H
#define FPPA_PATHFINDING_H
#include <deque>
#include <vector>
#include "utilities/point.h"

class Robot;
class GameState;


/*! @brief Fast Path Planning Algorithm
 * @author JamesW, Muhaimen Shamsi
 * @details This namespace implements the Fast Path Planning Algorithm (FPPA)
 * as described in the [paper](http://fei.edu.br/rcs/2014/RegularPapers/robocupsymposium2014_submission_55.pdf)
 * "Fast Path Planning Algorithm for the RoboCub Small Size League" by Rodriguez S., Rojas E., Calderon J. et al.
 * The algorithm is meant to provide reliable path-finding in real time, such as in
 * the case of the RoboCup league. */

namespace FPPA
{
    //! @brief A vector of waypoints representing a path
    typedef std::deque<Point> Path;

    /*! @brief Updates the list of points that represent robot obstacles
     * @details Ignores the robot that is passed in when considering obstacles
     * @param self Pointer to the robot that obstacles are being retrieved for.
     */
    void updateRobotObstacles(Robot* self, GameState* game_state);

    /*! @brief Use FPPA to find a path from Point `start` to Point `end`.
     * @details Returns a std::pair of a the path waypoint vector and a
     * PathDirection indicating which direction was chosen.
     * @param start The starting point of the path
     * @param end The ending point of the path
     * @param avoidBall Controls counting the ball as an obstacle or not
     * @param pathHint Prioritizes selecting top or bottom path regardless of distance.
     * @param unlessValue An `unless-than value` speificies: Choose the pathHint
     *  path UNLESS the shorter path is "unlessValue" percent better than the
     *  requested top/bottom path.
     * @see PathInfo */
    Path genPath(GameState* game_state, const Point& start, Point end, bool avoidBall = true, bool is_goalie = true);

    /*! @brief Check for an obstacle in a line
     * @details Check if there is an obstacle (robot or ball) in the
     * line defined from start to end. if obsPosOut is not a nullptr, it will be filled
     * with the found obstacle position if this function returns true.
     * @param start The line starting point
     * @param end The line ending point
     * @param obsPosOut If non-nullptr and an obstacle is found, this is filled with its position
     * @param avoidBall to count the ball as an obstacle or not (true by default). */
    bool isObstacleInLine(GameState* game_state,
                          const Point& start,
                          const Point& end,
                          Point *obsPosOut = nullptr,
                          bool avoidBall = true);
}



#endif // FPPA_PATHFINDING_H
