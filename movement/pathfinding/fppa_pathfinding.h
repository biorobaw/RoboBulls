#ifndef FPPA_PATHFINDING_H
#define FPPA_PATHFINDING_H
#include <vector>
#include "utilities/point.h"

/*! @brief Fast Path Planning Algorithm
 * @author JamesW
 *
 * This namespace implements the Fast Path Planning Algorithm (FPPA)
 * as described in the paper "Fast Path Planning Algorithm for the RoboCub Small Size League"
 * by Rodriguez S., Rojas E., Calderon J. et al.
 * The algorithm is meant to provide reliable path-finding in real time, such as in
 * the case of the RoboCup league. */
namespace FPPA
{
    //! @brief Represents weather the chosen path was the top or the bottom one
    enum class PathDirection {
        None, Bottom, Top
    };

    //! @brief A vector of waypoints representing a clear path
    typedef std::vector<Point> Path;

    //! @brief Path information returned by findShortestPath
    typedef std::pair<Path, PathDirection> PathInfo;

    /****************************************************************/

    /*! @brief Updates currently-considered obstacles
     * @details If FRAME_UPDATE_COUNT calls are made, re-builds the currently
     * considered obstacles. and updates the state of the field */
    void update();

    /****************************************************************/

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
    PathInfo findShortestPath(const Point& start, const Point& end,
                              bool avoidBall = true,
                              PathDirection pathHint = PathDirection::None,
                              float unlessValue = -1.0);

    /*! @brief Check for an obstacle in a line
     * @details Check if there is an obstacle (robot or ball) in the
     * line defined from start to end. if obsPosOut is not a nullptr, it will be filled
     * with the found obstacle position if this function returns true.
     * @param start The line starting point
     * @param end The line ending point
     * @param obsPosOut If non-nullptr and an obstacle is found, this is filled with its position
     * @param avoidBall to count the ball as an obstacle or not (true by default). */
    bool isObstacleInLine(const Point& start,
                          const Point& end,
                          Point *obsPosOut = nullptr,
                          bool avoidBall = true);

    /*! @brief Test if a given single point in question is an obstacle in a given line
     *  @param start The line starting point
     *  @param end The line ending point
     *  @param question The point to test */
    bool isPointInLine(const Point& start, const Point& end, const Point& question);


    /*! @brief Get all current considered obstacles
     *  @details Returns a reference to the vector consisting of the positions of all
     *  obstacles for this frame */
    const std::vector<Point>& getCurrentObstacles();
}

#endif // FPPA_PATHFINDING_H
